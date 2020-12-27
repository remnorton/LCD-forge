#include <QDebug>
#include <QFileDialog>
#include <Qsci/qscilexercpp.h>
#include "wgtimagetool.h"
#include "imagedatadialog.h"
#include "bitmaptresdialog.h"
#include "config.h"

#include <vector>
#include <QMap>

#pragma pack(1)
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t unused;
}RGB_t;
#pragma pack()

wgtImageTool::wgtImageTool(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
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
    splitter->setSizes(QList<int>() << 30000 << 70000);
    gwPreview->setScene(&scene);
}

void wgtImageTool::on_btImage_clicked()
{
    ImageDataDialog dlg;
    if (dlg.exec() != QDialog::Accepted) return;
    uint8_t bpp = dlg.getBitPerPixel();
    uint8_t c_order = dlg.getColorOrder();
    QString Content_c;
    for (int y = 0; y < source.height(); y++)
    {
        QString line;
        for (int x = 0; x < source.width(); x++)
        {
            QRgb p = source.pixel(x,y);
            RGB_t pix;
            if (!line.isEmpty()) line += ", ";
            switch (bpp)
            {
                case 16:
                {
                    pix.r = qRed(p) >> 3;
                    pix.g = qGreen(p) >> 2;
                    pix.b = qBlue(p) >> 3;

                    uint16_t pixel;
                    if (c_order == CO_BGR)
                        pixel = (pix.r << 11) | (pix.g << 5) | (pix.b);
                    else
                        pixel = (pix.b << 11) | (pix.g << 5) | (pix.r);
                    line += QString("0x%2, 0x%1").arg((pixel&0xff), 2, 16, QChar('0')).arg((pixel&0xff00)>>8, 2, 16, QChar('0'));
                    break;
                }
                case 18:
                {
                    pix.r = qRed(p) >> 2;
                    pix.g = qGreen(p) >> 2;
                    pix.b = qBlue(p) >> 2;

                    uint32_t pixel;
                    if (c_order == CO_BGR)
                        pixel = (pix.r << 12) | (pix.g << 6) | (pix.b);
                    else
                        pixel = (pix.b << 12) | (pix.g << 6) | (pix.r);
                    line += QString("0x%3, 0x%2, 0x%1").arg((pixel&0xff), 2, 16, QChar('0')).arg((pixel&0xff00)>>8, 2, 16, QChar('0')).arg((pixel&0xff0000)>>16, 2, 16, QChar('0'));
                    break;
                }
                case 24:
                {
                    pix.r = qRed(p);
                    pix.g = qGreen(p);
                    pix.b = qBlue(p);
                    if (c_order == CO_BGR)
                        line += QString("0x%1, 0x%2, 0x%3").arg(pix.b, 2, 16, QChar('0')).arg(pix.g, 2, 16, QChar('0')).arg(pix.r, 2, 16, QChar('0'));
                    else
                        line += QString("0x%1, 0x%2, 0x%3").arg(pix.r, 2, 16, QChar('0')).arg(pix.g, 2, 16, QChar('0')).arg(pix.b, 2, 16, QChar('0'));
                    break;
                }
            }
        }
        if (!Content_c.isEmpty()) Content_c += ",\r\n";
        Content_c += line;
    }
    QFileInfo finfo = QFileInfo(edImage->text());
    QString fname = finfo.baseName();
    QString bmp_name = fname;
    bmp_name.replace(" ","_").replace("-","_");
    bmp_name += "_bitmap";

    Content_c = QString("/*\r\n* Image data for '%7'\r\n*/\r\n\r\nconst uint8_t %1[] = {\r\n%2\r\n};\r\n\r\n DSP_Image_t %1_image = {\r\n%3 /*width*/,\r\n%4, /*height*/\r\n%5, /*bit_per_pixel*/\r\n%6 /*data*/\r\n};\r\n")
            .arg(bmp_name)
            .arg(Content_c)
            .arg(source.width())
            .arg(source.height())
            .arg(bpp)
            .arg(bmp_name)
            .arg(finfo.fileName());

    editor->setText(Content_c);
}

void wgtImageTool::on_btOpenImage_clicked()
{
    //QString fName = QFileDialog::getOpenFileName(this, "Open image", "", "All image files (*.jpg *.png *.bmp);;PNG images (*.png);;JPEG images (*.jpg);;Windows bitmap (*.bmp)");
    QString fName = QFileDialog::getOpenFileName(this, "Open image", "", "JPEG images (*.jpg)");
    if (fName.isEmpty()) return;
    edImage->setText(fName);
    source.load(fName);
    scene.clear();
    scene.addPixmap(QPixmap::fromImage(source));
    gwPreview->update();
    frButtons->setEnabled(true);
}

void wgtImageTool::on_btBitmap_clicked()
{
    uint8_t tres = QConfig::getValue("IMG_TOOL/Treshold").toUInt();
    BitmapTresDialog dlg;
    if (tres) dlg.setTreshold(tres);
    if (dlg.exec() != QDialog::Accepted) return;
    tres =  dlg.getTreshold();
    QConfig::setValue("IMG_TOOL/Treshold",tres);
    Content_c.clear();
    QList<QByteArray> lines;
    for (int y = 0; y < source.height(); y++)
    {
        QByteArray line;
        for (int x = 0; x < source.width(); x++)
        {
            QRgb p = source.pixel(x,y);
            RGB_t pix;
            pix.r = qRed(p);
            pix.g = qGreen(p);
            pix.b = qBlue(p);

            if ((pix.r < tres) && (pix.g < tres) && (pix.b < tres)) line.append('1');
            else line.append('0');

        }
        //Content_c += line + ",\r\n";
        lines.append(line);
    }

    if (lines.isEmpty()) return;

    foreach(QByteArray l, lines)
    {
        QString comment = "//\t"+QString(l).replace("0"," ").replace("1", "#")+"\r\n";
        QString code;
        while (l.count())
        {
            uint8_t bit_to_copy = (l.count() > 8)?8:l.count();
            QByteArray b = l.left(bit_to_copy);
            while (b.count() < 8) b.append('0');
            l.remove(0, bit_to_copy);
            uint8_t bt = QString(b).toUInt(0,2);
            code += QString("0x%1,").arg(bt,2,16,QChar('0'));
        }

        Content_c += code+comment;
    }
    QFileInfo finfo = QFileInfo(edImage->text());
    QString fname = finfo.baseName();
    QString bmp_name = fname;
    bmp_name.replace(" ","_").replace("-","_");

    Content_c = QString("/*\r\n* Bitmap data for '%1'\r\n*/\r\n\r\nconst uint8_t %2_bytes[] = {\r\n%3\r\n};\r\n\r\nDSP_Bitmap_t %2_bitmap = {\r\n%4, /*width*/\r\n%5, /*height*/\r\n%2_bytes /*data*/\r\n};\r\n")
            .arg(finfo.fileName())
            .arg(bmp_name)
            .arg(Content_c)
            .arg(source.width())
            .arg(source.height());

    editor->setText(Content_c);
}
