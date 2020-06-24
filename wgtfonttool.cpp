/*
 * wgtfonttool.cpp
 *
 *  Created on: 21.10.2018
 *		Author: Rudenko Roman (aka Rem Norton)
 *
 */

#include "config.h"
#include "wgtfonttool.h"
#include <QDebug>
#include <QFont>
#include <QFontDialog>
#include <QPainter>
#include <QBitmap>
#include <QBuffer>
#include <QStaticText>
#include <QRgb>
#include <QTextItem>
#include <QImage>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <Qsci/qscilexercpp.h>
#include "Qsci/qsciscintilla.h"

enum
{
    CST_FULL = 0,
    CST_LATIN,
    CST_DIGITS10,
    CST_DIGITS16,
    CST_EXTENDED,
    CST_BASIC,
    CST_AUX
};


static QMap<int, QString> presetNames =
{
    {CST_FULL,"All local 8-bit table symbols"},
    {CST_BASIC,"Basic ASCII printable symbols (32~127)"},
    {CST_EXTENDED,"Extended ASCII symbols (128~255)"},
    {CST_LATIN,"Latin chars"},
    {CST_DIGITS10,"Digits"},
    {CST_DIGITS16,"Hex digits"},
    {CST_AUX,"Auxiliary symbols"}
};

//
//
//

wgtFontTool::wgtFontTool(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    QString defFont = QConfig::getValue("FNT_TOOL/Font").toString();
    if (curFont.fromString(defFont))
        setCurFont(curFont);

    //setup editor
    editor = new QsciScintilla(this);
    editor->setUtf8(true);
    editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setReadOnly(true);
    editor->setMarginType(1, QsciScintilla::NumberMargin);
    editor->setMarginLineNumbers(1, true);
    editor->setMarginWidth(1,30);
    QsciLexerCPP* lexer = new QsciLexerCPP;
    lexer->setFont(QFont("Courier New",11));
    lexer->setFoldComments(true);
    lexer->setFoldCompact(true);

    editor->setLexer(lexer);
    editor->setAutoCompletionSource(QsciScintilla::AcsAll);

    glSci->addWidget(editor);
    splitter->setSizes(QList<int>() << 30000 << 70000);

    gbtOutput->setId(rbSource, 1);
    gbtOutput->setId(rbHeaders, 3);
    gbtOutput->setId(rbTypedefs, 2);

    QMap<int, QString>::iterator i = presetNames.begin();
    while(i != presetNames.end())
    {
        cbCharset->addItem(i.value(), i.key());
        i++;
    }

}

void wgtFontTool::drawASCII()
{
    QString content = tbSample->toPlainText();
    QByteArray ascii;

    switch(cbCharset->currentData().toUInt())
    {
        case CST_FULL: //all
        {
            ascii.resize(256-32);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 32; i < 256; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            break;
        }
        case CST_BASIC:
        {
            ascii.resize(128-32);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 32; i < 128; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            break;
        }
        case CST_LATIN:
        {
            ascii.resize(26*2);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 65; i < 91; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            for (int i = 97; i < 122; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            break;
        }
        case CST_DIGITS10:
        {
            ascii.resize(10);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 48; i < 58; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            break;
        }
        case CST_DIGITS16:
        {
            ascii.resize(17);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 48; i < 58; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            for (int i = 65; i < 71; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }

            *chr = 120; //'x'

            break;
        }
        case CST_EXTENDED:
        {
            ascii.resize(256-128);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 128; i < 256; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            break;
        }
        case CST_AUX:
        {
            ascii.resize(16+7+6+4);
            uint8_t* chr = (uint8_t*) ascii.data();
            for (int i = 32; i < 48; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            for (int i = 58; i < 65; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            for (int i = 91; i < 97; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            for (int i = 123; i < 127; i++)
            {
                *chr = (uint8_t)i;
                chr++;
            }
            break;
        }
    }

    content += QString::fromLocal8Bit(ascii);
    tbSample->setPlainText(content);
}

void wgtFontTool::fillFontMap()
{
    fontMap.clear();
    QString charset = tbSample->toPlainText();
    if (charset.isEmpty()) return;

    QByteArray charList = charset.toLocal8Bit();
    //qDebug() << charList;

    //calc max width
    uint16_t maxW = 0;
    for(int c = 0; c < charList.count(); c++)
    {
        uint8_t i = (uint8_t)charList[c];
        QString sym = QString::fromLocal8Bit((const char*)&i, 1);
        if ((i == 160) || (i == 173)) sym = " ";
        QFontMetrics mt(curFont);
        if (maxW < mt.horizontalAdvance(sym)) maxW = mt.horizontalAdvance(sym);
    }

    for(int c = 0; c < charList.count(); c++)
    {
        uint8_t i = (uint8_t)charList[c];

        curFont.setStyleStrategy(QFont::NoAntialias);

        //generate pixmap
        QString sym = QString::fromLocal8Bit((const char*)&i, 1);
        if ((i == 160) || (i == 173)) sym = " ";

        QFontMetrics mt(curFont);
        int width = maxW;
        int height = mt.height();
        int btWidth = width;
        if (btWidth%8) btWidth = (btWidth/8+1)*8;


        QImage img(QSize(btWidth, height), QImage::Format_RGB32);
        img.fill(QColor(Qt::white));

        QPainter painter(&img);

        painter.beginNativePainting();
        painter.setFont(curFont);

        curFont.setStyleStrategy(QFont::NoAntialias);
        painter.setPen(QPen(QBrush(QColor(0,0,0)),1,Qt::SolidLine));
        painter.drawText(QRect(0,0,btWidth,height),0,sym);

        painter.endNativePainting();

//        img.save(QString("c:/fnt_work/%1_0.bmp").arg(i));

        CharInfo_t cInfo = {0,0,0};

        for (int h = 0; h < height; h++)
        {
            QByteArray line;
            for (int w = 0; w < btWidth; w++)
            {
                if (img.pixel(w,h) <= qRgb(128,128,128)) line.append('1');
                else line.append('0');
            }
            cInfo.lines.append(line);
        }
        cInfo.width = width;

        fontMap.insert(i, cInfo);
    }
}

void wgtFontTool::setCurFont(QFont fnt)
{
    curFont = fnt;
    QFontInfo fInfo(curFont);
    edFont->setText(QString("\"%1\" %2pt %3").arg(fInfo.family()).arg(fInfo.pointSize()).arg(fInfo.styleName()));
    tbSample->setFont(curFont);
    QConfig::setValue("FNT_TOOL/Font",fnt.toString());
}

void wgtFontTool::optimizeFontMap()
{
    uint8_t minTrimUp = 255;
    uint8_t minTrimDown = 255;
    foreach(uint8_t i, fontMap.keys())
    {
        CharInfo_t info = fontMap.value(i);
        if (i == 32)
        {
            info.trimDown = info.lines.count()-1;
            while (info.lines.count() > 1) info.lines.removeFirst();
            info.lines[0] = info.lines.first().left(8);
            info.width = sbSpaceWidth->value();
        }
        else
        {
            optimizeChar(&info);
            minTrimDown = qMin(minTrimDown, info.trimDown);
            minTrimUp = qMin(minTrimUp, info.trimUp);
            //qDebug() << Q_FUNC_INFO << i << minTrimDown << minTrimUp << info.trimDown << info.trimUp;
        }

        fontMap.insert(i, info);
    }

    if (minTrimUp || minTrimDown)
    {
        foreach(uint8_t i, fontMap.keys())
        {
            CharInfo_t info = fontMap.value(i);
            if (i != 32)
            {
                info.trimDown -= minTrimDown;
                info.trimUp -= minTrimUp;
                fontMap.insert(i, info);
            }
        }
        heightDec = minTrimDown + minTrimUp;
    }
}

void wgtFontTool::generateCode()
{
    QString descriptorsTable/* = QString("{0,%1}, //! <Space> \r\n").arg(spaceWidth)*/;
    QString bitmapTable;
    uint16_t offset = 0;
    QStringList dscList;
    rawBitmap.clear();
    rawDesc.clear();
    for (int i = 0; i < 256; i++ )
    {
        if (fontMap.keys().contains((uint8_t)i))
        {
            if (!start) start = i;
            stop = i;
            CharInfo_t info = fontMap.value(i);
            if (!bitmapTable.isEmpty()) bitmapTable += "\r\n";
            bitmapTable += QString("/*Bitmap for char '%1' (%2)*/\r\n").arg(QString::fromLocal8Bit((const char*)&i, 1)).arg(i);
            uint16_t size = 0;
            foreach(QByteArray line, info.lines)
            {
                QString bytes;
                QString comment;
                uint8_t tabs = 5;
                while (line.count())
                {
                    QByteArray b = line.left(8);
                    line.remove(0,8);
                    size++;
                    if (tabs) tabs--;
                    uint8_t bt = QString(b).toUInt(0, 2);
                    rawBitmap.append((char)bt);
                    bytes += QString("0x%1,").arg(bt,2,16,QChar('0'));
                    b = b.replace('0',' ').replace('1','#');
                    comment += QString(b);
                }

                bitmapTable += "\t" + bytes;
                for (int t = 0; t < tabs; t++) bitmapTable += "\t";
                bitmapTable += "//" + comment + "\r\n";
            }
            dscList.append(QString("\t{%1, %2, %3, %4},\t//Descriptor for '%5' (%6)")
                                .arg(offset)
                                .arg(info.width)
                                .arg(info.lines.count())
                                .arg(info.trimDown)
                                .arg(QString::fromLocal8Bit((const char*)&i, 1)).arg(i)
                           );
            Font_Chart_info_t fci;
            fci.offset = offset;
            fci.width = info.width;
            fci.height = info.lines.count();
            fci.trimed = info.trimDown;

            rawDesc.append(QByteArray((const char*)&fci, sizeof(fci)));

            offset += size;
        }
        else
        {
            if (start)
            {
                dscList.append("\t{0, 0, 0, 0},");
                if (!spaced) spaced = i;
            }
        }

    }

    while(dscList.count() > (stop-start+1)) dscList.removeLast();
    descriptorsTable = dscList.join("\r\n");

    QFontInfo fInfo(curFont);
    QString fntName = fInfo.family().replace(' ','_').replace("-","_")+"_"+QString("%1").arg(fInfo.pointSize())+"pt";
    QString btaName = QString("%1_Bitmaps")
                      .arg(fntName);
    QString dsaName = QString("%1_Descriptors").arg(fntName);


    //qDebug() << Q_FUNC_INFO << start << stop << spaced << ((spaced > 0) && (spaced < stop)) << monospace;
    bool dsc_no_generate = (monospace && !((spaced > 0) && (spaced < stop)));
    descriptorsTable = dsc_no_generate?"":QString("Font_Char_info_t %1[]=\r\n{\r\n%2\r\n}; //Local 8-bit codepage table descriptors").arg(dsaName).arg(descriptorsTable);


    bitmapTable = QString("uint8_t %1[]=\r\n{\r\n%2\r\n}; //Local 8-bit codepage bitmaps")
                  .arg(btaName)
                  .arg(bitmapTable);

    QString fntPublicName;
    if (fInfo.family().length() > 11) fntPublicName = QString("%1~%2pt").arg(fInfo.family().left(10)).arg(fInfo.pointSize());
    else fntPublicName = QString("%1 %2pt").arg(fInfo.family().left(11)).arg(fInfo.pointSize());

    memset(fileHdr.name, 0, sizeof(fileHdr.name));
    memcpy(fileHdr.name, fntPublicName.toLocal8Bit().data(), fntPublicName.count());
    fileHdr.width = dsc_no_generate?fontMap.value(fontMap.keys().first()).width:0;
    fileHdr.height = QFontMetrics(curFont).height()-heightDec;
    fileHdr.start = start;
    fileHdr.stop = stop;
    fileHdr.spacing = dsc_no_generate?0:sbSpacing->value();

    QString fntDef = QString("Font_type_t font_%1  = \r\n{\r\n\t\"%2\", /*name*/\r\n\t%3, /*bitmaps*/\r\n\t%4, /*descr*/\r\n\t%5, /*width*/\r\n\t%6, /*height*/\r\n\t%8, /*start*/\r\n\t%9, /*stop*/\r\n\t%7, /*spacing*/\r\n}; ")
                     .arg(fntName)
                     .arg(fntPublicName)
                     .arg(btaName)
                     .arg(dsc_no_generate?"0":dsaName)
                     .arg(fileHdr.width)
                     .arg(fileHdr.height)
                     .arg(fileHdr.spacing)
                     .arg(start)
                     .arg(stop);



    contentC = QString("/*\r\n* Font data for '%1 %2pt'\r\n*/\r\n#include\"fonts_lib.h\" \r\n").arg(fInfo.family()).arg(fInfo.pointSize())+bitmapTable + "\r\n\r\n"+descriptorsTable + "\r\n\r\n" + fntDef;
    QFile typedefs(":/Files/TYPEDEFS");
    if (typedefs.open(QFile::ReadOnly))
        contentH = QString(typedefs.readAll());
    typedefs.close();

    QString defmark = QString("_%1_").arg(fntName.toUpper());
    contentE = QString("#ifndef %1\r\n#define %1\r\n\r\n#include \"fonts_lib.h\";\r\n\r\nextern Font_type_t font_%2;\r\n\r\n#endif //%1")
               .arg(defmark)
               .arg(fntName);

    onChangeOutput();
    btSaveBin->setDisabled(contentC.isEmpty());
}

void wgtFontTool::optimizeChar(CharInfo_t* info)
{
    uint8_t leftSpace = 255;
    uint8_t rightSpace = 255;
    uint8_t* trim = &info->trimUp;

    for (int i = 0; i < info->lines.count(); i++)
    {
        QByteArray line = info->lines[i];

        if (!line.contains('1')) //empty line
            (*trim)++;
        else
        {
            trim = &info->trimDown;
            info->trimDown = 0;
            int16_t idx = line.indexOf('1');
            leftSpace = qMin((uint8_t)idx,leftSpace);
            int16_t last = idx;
            while (last >= 0)
            {
                idx = qMax(last,idx);
                last = line.indexOf('1',last+1);
            }
            idx = line.count() - idx - 1;
            rightSpace = qMin((uint8_t)idx,rightSpace);
        }
        //qDebug() << line << leftSpace << rightSpace;
    }

    if (!chkTrimUp->isChecked()) info->trimUp = 0;
    if (!chkTrimDown->isChecked()) info->trimDown = 0;

    //trim. up
    for (int i = 0; i < info->trimUp; i++)
        info->lines.removeFirst();

    //trim down
    for (int i = 0; i < info->trimDown; i++)
        info->lines.removeLast();

    leftSpace = chkTrimLeft->isChecked()?leftSpace:0;
    rightSpace = chkTrimRight->isChecked()?rightSpace:0;

    //trim left & right
    if (leftSpace + rightSpace)
    {
        if (info->lines.count())
            info->width = info->lines.first().count()-leftSpace-rightSpace;

        for (int i = 0; i < info->lines.count(); i++)
        {
            QByteArray l = info->lines.first();
            info->lines.removeFirst();
            l = l.mid(leftSpace,l.count()-rightSpace-leftSpace);
            while(l.count()%8) l.append('0');
            info->lines.append(l);
        }
    }

    //qDebug() << Q_FUNC_INFO << leftSpace << rightSpace << info->width;
}

void wgtFontTool::onChangeOutput()
{
    switch(gbtOutput->checkedId())
    {
        case 1: //main source
        {
            editor->setText(contentC);
            lbContent->setText("Source file (.c/.cpp)");
            break;
        }
        case 2: //typedefs
        {
            editor->setText(contentH);
            lbContent->setText("Type definition header (.h)");
            break;
        }
        case 3: //header
        {
            editor->setText(contentE);
            lbContent->setText("");
            break;
        }
    }
}

void wgtFontTool::on_btCreate_clicked()
{

    editor->clear();
    spaced = start = stop = 0;
    heightDec = 0;
    monospace = !(chkTrimDown->isChecked() || chkTrimLeft->isChecked() || chkTrimRight->isChecked() || chkTrimUp->isChecked());

    //generate bitmaps
    fillFontMap();

    //optimize bitmaps
    if (!monospace)
        optimizeFontMap();

    //generate code
    generateCode();

}

void wgtFontTool::on_btSelectFont_clicked()
{
    QFontDialog dlg;
    dlg.setCurrentFont(curFont);
    if (dlg.exec() == QDialog::Accepted)
        setCurFont(dlg.currentFont());
}

void wgtFontTool::on_btAddPreset_clicked()
{
    drawASCII();
}

void wgtFontTool::on_tbSample_textChanged()
{
    btCreate->setDisabled(tbSample->toPlainText().isEmpty());
}

void wgtFontTool::on_btSaveBin_clicked()
{
    QString fName = QFileDialog::getSaveFileName(this, "Save binary font", "", "Font files (*.fnt)");
    if (fName.isEmpty()) return;
    if (QFileInfo(fName).suffix().toLower() != "fnt")
        fName += ".fnt";

    memcpy(fileHdr.prefix,(uint8_t*)"FNT", 3);

    QFile fl(fName);
    if (fl.open(QFile::WriteOnly))
    {
        //write header
        fl.write((const char*)&fileHdr, sizeof(fileHdr));

        //write descriptors
        fl.write(rawDesc);

        //write bitmaps
        fl.write(rawBitmap);
    }
    fl.close();
}
