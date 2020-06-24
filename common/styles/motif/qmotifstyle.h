/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMOTIFSTYLE_H
#define QMOTIFSTYLE_H

#include <QtWidgets/qcommonstyle.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qpointer.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QPalette;
class QFocusFrame;
class QProgressBar;

class QMotifStyle : public QCommonStyle
{
    Q_OBJECT

public:
    explicit QMotifStyle(bool useHighlightCols=false);
    virtual ~QMotifStyle();

    void setUseHighlightColors(bool);
    bool useHighlightColors() const;

    void polish(QPalette&) Q_DECL_OVERRIDE;
    void polish(QWidget*) Q_DECL_OVERRIDE;
    void unpolish(QWidget*) Q_DECL_OVERRIDE;
    void polish(QApplication*) Q_DECL_OVERRIDE;
    void unpolish(QApplication*) Q_DECL_OVERRIDE;

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
                        const QWidget *w = 0) const Q_DECL_OVERRIDE;

    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                      const QWidget *w = 0) const Q_DECL_OVERRIDE;

    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p,
                            const QWidget *w = 0) const Q_DECL_OVERRIDE;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = 0,
                     const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    QSize sizeFromContents(ContentsType ct, const QStyleOption *opt,
                           const QSize &contentsSize, const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    QRect subElementRect(SubElement r, const QStyleOption *opt,const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    int styleHint(StyleHint hint, const QStyleOption *opt = 0, const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const Q_DECL_OVERRIDE;

    bool event(QEvent *) Q_DECL_OVERRIDE;
    QPalette standardPalette() const Q_DECL_OVERRIDE;
    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *opt = 0,
                       const QWidget *widget = 0) const Q_DECL_OVERRIDE;

protected:
    QPointer<QFocusFrame> focus;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;
    void startProgressAnimation(QProgressBar *bar);
    void stopProgressAnimation(QProgressBar *bar);

private:
    bool highlightCols;
    QList<QProgressBar *> bars;
    int animationFps;
    int animateTimer;
    QTime startTime;
    int animateStep;

protected:
    int spinboxHCoeff;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QMOTIFSTYLE_H
