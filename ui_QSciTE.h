/********************************************************************************
** Form generated from reading UI file 'QSciTE.ui'
**
** Created: Tue Feb 25 21:03:42 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSCITE_H
#define UI_QSCITE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QSciTE
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QTabWidget *tabBarLeft;
    QTabWidget *tabBarRight;
    QFrame *searchFrame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_13;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *searchField;
    QSpacerItem *horizontalSpacer;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *previousButton;
    QSpacerItem *horizontalSpacer_4;
    QFrame *replaceFrame;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_11;
    QLabel *replaceFindLabel;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *replaceSearchField;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *replaceNextButton;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_12;
    QLabel *replaceLabel;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *replaceField;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *replaceButton;
    QSpacerItem *horizontalSpacer_10;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *QSciTE)
    {
        if (QSciTE->objectName().isEmpty())
            QSciTE->setObjectName(QString::fromUtf8("QSciTE"));
        QSciTE->resize(829, 438);
        centralwidget = new QWidget(QSciTE);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setFocusPolicy(Qt::NoFocus);
        centralwidget->setAcceptDrops(false);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setLineWidth(0);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(3);
        tabBarLeft = new QTabWidget(splitter);
        tabBarLeft->setObjectName(QString::fromUtf8("tabBarLeft"));
        tabBarLeft->setFocusPolicy(Qt::ClickFocus);
        tabBarLeft->setAcceptDrops(false);
        tabBarLeft->setTabPosition(QTabWidget::North);
        tabBarLeft->setTabShape(QTabWidget::Triangular);
        tabBarLeft->setElideMode(Qt::ElideRight);
        tabBarLeft->setUsesScrollButtons(true);
        tabBarLeft->setTabsClosable(true);
        tabBarLeft->setMovable(true);
        splitter->addWidget(tabBarLeft);
        tabBarRight = new QTabWidget(splitter);
        tabBarRight->setObjectName(QString::fromUtf8("tabBarRight"));
        tabBarRight->setFocusPolicy(Qt::ClickFocus);
        tabBarRight->setTabShape(QTabWidget::Triangular);
        tabBarRight->setUsesScrollButtons(true);
        tabBarRight->setTabsClosable(true);
        tabBarRight->setMovable(true);
        splitter->addWidget(tabBarRight);

        verticalLayout->addWidget(splitter);

        searchFrame = new QFrame(centralwidget);
        searchFrame->setObjectName(QString::fromUtf8("searchFrame"));
        searchFrame->setEnabled(true);
        searchFrame->setFrameShape(QFrame::NoFrame);
        searchFrame->setFrameShadow(QFrame::Plain);
        searchFrame->setLineWidth(0);
        horizontalLayout = new QHBoxLayout(searchFrame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 0, 5, 0);
        horizontalSpacer_13 = new QSpacerItem(10, 10, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_13);

        label = new QLabel(searchFrame);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setMinimumSize(QSize(52, 0));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        searchField = new QLineEdit(searchFrame);
        searchField->setObjectName(QString::fromUtf8("searchField"));
        searchField->setAutoFillBackground(false);
        searchField->setFrame(false);

        horizontalLayout->addWidget(searchField);

        horizontalSpacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        nextButton = new QPushButton(searchFrame);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setFocusPolicy(Qt::NoFocus);
        nextButton->setFlat(true);

        horizontalLayout->addWidget(nextButton);

        horizontalSpacer_2 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        previousButton = new QPushButton(searchFrame);
        previousButton->setObjectName(QString::fromUtf8("previousButton"));
        previousButton->setFlat(true);

        horizontalLayout->addWidget(previousButton);

        horizontalSpacer_4 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(searchFrame);

        replaceFrame = new QFrame(centralwidget);
        replaceFrame->setObjectName(QString::fromUtf8("replaceFrame"));
        replaceFrame->setFrameShape(QFrame::NoFrame);
        replaceFrame->setFrameShadow(QFrame::Plain);
        replaceFrame->setLineWidth(0);
        verticalLayout_3 = new QVBoxLayout(replaceFrame);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 0, 5, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_11 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_11);

        replaceFindLabel = new QLabel(replaceFrame);
        replaceFindLabel->setObjectName(QString::fromUtf8("replaceFindLabel"));
        sizePolicy2.setHeightForWidth(replaceFindLabel->sizePolicy().hasHeightForWidth());
        replaceFindLabel->setSizePolicy(sizePolicy2);
        replaceFindLabel->setMinimumSize(QSize(52, 0));
        replaceFindLabel->setLayoutDirection(Qt::LeftToRight);
        replaceFindLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(replaceFindLabel);

        horizontalSpacer_5 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        replaceSearchField = new QLineEdit(replaceFrame);
        replaceSearchField->setObjectName(QString::fromUtf8("replaceSearchField"));

        horizontalLayout_2->addWidget(replaceSearchField);

        horizontalSpacer_6 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        replaceNextButton = new QPushButton(replaceFrame);
        replaceNextButton->setObjectName(QString::fromUtf8("replaceNextButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(replaceNextButton->sizePolicy().hasHeightForWidth());
        replaceNextButton->setSizePolicy(sizePolicy3);
        replaceNextButton->setMinimumSize(QSize(100, 0));
        replaceNextButton->setBaseSize(QSize(0, 0));
        replaceNextButton->setFocusPolicy(Qt::NoFocus);
        replaceNextButton->setFlat(true);

        horizontalLayout_2->addWidget(replaceNextButton);

        horizontalSpacer_9 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_12 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_12);

        replaceLabel = new QLabel(replaceFrame);
        replaceLabel->setObjectName(QString::fromUtf8("replaceLabel"));
        sizePolicy2.setHeightForWidth(replaceLabel->sizePolicy().hasHeightForWidth());
        replaceLabel->setSizePolicy(sizePolicy2);
        replaceLabel->setMinimumSize(QSize(50, 0));
        replaceLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(replaceLabel);

        horizontalSpacer_7 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        replaceField = new QLineEdit(replaceFrame);
        replaceField->setObjectName(QString::fromUtf8("replaceField"));

        horizontalLayout_3->addWidget(replaceField);

        horizontalSpacer_8 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        replaceButton = new QPushButton(replaceFrame);
        replaceButton->setObjectName(QString::fromUtf8("replaceButton"));
        sizePolicy3.setHeightForWidth(replaceButton->sizePolicy().hasHeightForWidth());
        replaceButton->setSizePolicy(sizePolicy3);
        replaceButton->setMinimumSize(QSize(100, 0));
        replaceButton->setFocusPolicy(Qt::NoFocus);
        replaceButton->setFlat(true);

        horizontalLayout_3->addWidget(replaceButton);

        horizontalSpacer_10 = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_10);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout->addWidget(replaceFrame);

        QSciTE->setCentralWidget(centralwidget);
        searchFrame->raise();
        replaceFrame->raise();
        tabBarLeft->raise();
        tabBarRight->raise();
        splitter->raise();
        menubar = new QMenuBar(QSciTE);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 829, 22));
        menubar->setAcceptDrops(false);
        QSciTE->setMenuBar(menubar);
        statusbar = new QStatusBar(QSciTE);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QSciTE->setStatusBar(statusbar);

        retranslateUi(QSciTE);
        QObject::connect(searchField, SIGNAL(returnPressed()), nextButton, SLOT(click()));
        QObject::connect(replaceSearchField, SIGNAL(returnPressed()), replaceNextButton, SLOT(click()));
        QObject::connect(replaceField, SIGNAL(returnPressed()), replaceButton, SLOT(click()));

        QMetaObject::connectSlotsByName(QSciTE);
    } // setupUi

    void retranslateUi(QMainWindow *QSciTE)
    {
        QSciTE->setWindowTitle(QApplication::translate("QSciTE", "QSciTE", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QSciTE", "Find:", 0, QApplication::UnicodeUTF8));
        searchField->setPlaceholderText(QApplication::translate("QSciTE", "find...", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("QSciTE", "next", 0, QApplication::UnicodeUTF8));
        previousButton->setText(QApplication::translate("QSciTE", "previous", 0, QApplication::UnicodeUTF8));
        replaceFindLabel->setText(QApplication::translate("QSciTE", "Find:", 0, QApplication::UnicodeUTF8));
        replaceSearchField->setPlaceholderText(QApplication::translate("QSciTE", "find...", 0, QApplication::UnicodeUTF8));
        replaceNextButton->setText(QApplication::translate("QSciTE", "next", 0, QApplication::UnicodeUTF8));
        replaceLabel->setText(QApplication::translate("QSciTE", "Replace:", 0, QApplication::UnicodeUTF8));
        replaceField->setPlaceholderText(QApplication::translate("QSciTE", "replace...", 0, QApplication::UnicodeUTF8));
        replaceButton->setText(QApplication::translate("QSciTE", "replace", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QSciTE: public Ui_QSciTE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSCITE_H
