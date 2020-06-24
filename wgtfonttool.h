/*
 * wgtfonttool.h
 *
 *  Created on: 21.10.2018
 *		Author: Rudenko Roman (aka Rem Norton)
 *
 */


#ifndef WGTFONTTOOL_H
#define WGTFONTTOOL_H

#include "ui_wgtfonttool.h"
#include "Qsci/qsciscintilla.h"
//
//
//
typedef struct
{
    //uint32_t offset;
    uint8_t  width;
    uint8_t  trimUp;
    uint8_t  trimDown;
    QList<QByteArray> lines;
    QByteArray bitmap;
}CharInfo_t;

#pragma pack(1)
typedef struct
{
    uint16_t	offset;	//! offset in bitmap table
    uint8_t		width;	//! width of symbol
    uint8_t		height;	//! height of symbol
    uint8_t		trimed;	//! quantity of empty lines, removed from bottom
}Font_Chart_info_t;

typedef struct
{
    uint8_t prefix[3];          //! file prefix (FNT)
    uint8_t name[16];           //! table of bitmaps of symbols
    uint8_t width;              //! width of symbol. Ignored if table of descriptors are defined
    uint8_t height;             //! untrimmed symbols height
    uint8_t start;              //! code of first symbol in table
    uint8_t stop;               //! code of last symbol in table
    int8_t spacing;             //! spacing between symbols in line
}FontFile_Header_t;

#pragma pack()

typedef QMap<uint8_t, CharInfo_t> fontMap_t;

//
//
//
class wgtFontTool : public QWidget, private Ui::wgtFontTool
{
    Q_OBJECT

public:
    explicit wgtFontTool(QWidget *parent = nullptr);

private:
    void drawASCII();
    void setCurFont(QFont fnt);
    void fillFontMap();
    void optimizeFontMap();
    void generateCode();
    void optimizeChar(CharInfo_t* info);

private slots:
    void onChangeOutput();
    void on_btCreate_clicked();
    void on_btSelectFont_clicked();

    void on_btAddPreset_clicked();

    void on_tbSample_textChanged();

    void on_btSaveBin_clicked();

private:
    fontMap_t fontMap;
    QByteArray rawBitmap;
    QByteArray rawDesc;
    FontFile_Header_t fileHdr;
    QsciScintilla* editor;
    QFont curFont;
    QString contentC;
    QString contentH;
    QString contentE;
    bool monospace = false;
    uint8_t stop = 0;
    uint8_t start = 0;
    uint8_t spaced = 0;
    uint8_t heightDec = 0;

};

#endif // WGTFONTTOOL_H
