/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *surtidoresLabel;
    QLabel *empleadosLabel;
    QSpinBox *nSurtidores;
    QSpinBox *nEmpleados;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QTimeEdit *timeEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *timeLabel;
    QSpacerItem *horizontalSpacer_2;
    QLCDNumber *timeDisplay;
    QHBoxLayout *horizontalLayout_2;
    QLabel *comandoLabel;
    QLineEdit *comandoEditText;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *iniciarButton;
    QTextEdit *salidaEditText;
    QPushButton *limpiarButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(521, 502);
        MainWindow->setDocumentMode(true);
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        surtidoresLabel = new QLabel(centralWidget);
        surtidoresLabel->setObjectName(QStringLiteral("surtidoresLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, surtidoresLabel);

        empleadosLabel = new QLabel(centralWidget);
        empleadosLabel->setObjectName(QStringLiteral("empleadosLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, empleadosLabel);

        nSurtidores = new QSpinBox(centralWidget);
        nSurtidores->setObjectName(QStringLiteral("nSurtidores"));
        nSurtidores->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        formLayout->setWidget(0, QFormLayout::FieldRole, nSurtidores);

        nEmpleados = new QSpinBox(centralWidget);
        nEmpleados->setObjectName(QStringLiteral("nEmpleados"));
        nEmpleados->setButtonSymbols(QAbstractSpinBox::PlusMinus);

        formLayout->setWidget(1, QFormLayout::FieldRole, nEmpleados);


        verticalLayout_2->addLayout(formLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, -1, 0);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        timeEdit = new QTimeEdit(centralWidget);
        timeEdit->setObjectName(QStringLiteral("timeEdit"));
        timeEdit->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        timeEdit->setMaximumDate(QDate(2000, 1, 1));
        timeEdit->setCurrentSection(QDateTimeEdit::SecondSection);
        timeEdit->setCurrentSectionIndex(0);

        horizontalLayout_3->addWidget(timeEdit);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, -1);
        timeLabel = new QLabel(centralWidget);
        timeLabel->setObjectName(QStringLiteral("timeLabel"));
        timeLabel->setLayoutDirection(Qt::LeftToRight);
        timeLabel->setAutoFillBackground(true);
        timeLabel->setLineWidth(1);
        timeLabel->setScaledContents(false);
        timeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        timeLabel->setWordWrap(true);
        timeLabel->setMargin(0);
        timeLabel->setIndent(-1);
        timeLabel->setOpenExternalLinks(true);

        horizontalLayout_4->addWidget(timeLabel);

        horizontalSpacer_2 = new QSpacerItem(244, 30, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        timeDisplay = new QLCDNumber(centralWidget);
        timeDisplay->setObjectName(QStringLiteral("timeDisplay"));
        timeDisplay->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(timeDisplay->sizePolicy().hasHeightForWidth());
        timeDisplay->setSizePolicy(sizePolicy);
        timeDisplay->setMinimumSize(QSize(60, 0));
        timeDisplay->setFocusPolicy(Qt::StrongFocus);
        timeDisplay->setFrameShape(QFrame::Box);
        timeDisplay->setFrameShadow(QFrame::Plain);
        timeDisplay->setSmallDecimalPoint(false);
        timeDisplay->setDigitCount(5);
        timeDisplay->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_4->addWidget(timeDisplay);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        comandoLabel = new QLabel(centralWidget);
        comandoLabel->setObjectName(QStringLiteral("comandoLabel"));

        horizontalLayout_2->addWidget(comandoLabel);

        comandoEditText = new QLineEdit(centralWidget);
        comandoEditText->setObjectName(QStringLiteral("comandoEditText"));

        horizontalLayout_2->addWidget(comandoEditText);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        iniciarButton = new QPushButton(centralWidget);
        iniciarButton->setObjectName(QStringLiteral("iniciarButton"));
        iniciarButton->setAutoDefault(true);
        iniciarButton->setDefault(false);
        iniciarButton->setFlat(false);

        horizontalLayout->addWidget(iniciarButton);


        verticalLayout_2->addLayout(horizontalLayout);

        salidaEditText = new QTextEdit(centralWidget);
        salidaEditText->setObjectName(QStringLiteral("salidaEditText"));
        salidaEditText->setMouseTracking(true);
        salidaEditText->setFocusPolicy(Qt::NoFocus);
        salidaEditText->setFrameShadow(QFrame::Plain);
        salidaEditText->setReadOnly(true);

        verticalLayout_2->addWidget(salidaEditText);

        limpiarButton = new QPushButton(centralWidget);
        limpiarButton->setObjectName(QStringLiteral("limpiarButton"));
        limpiarButton->setAutoDefault(true);

        verticalLayout_2->addWidget(limpiarButton);

        MainWindow->setCentralWidget(centralWidget);
        QWidget::setTabOrder(iniciarButton, salidaEditText);
        QWidget::setTabOrder(salidaEditText, limpiarButton);

        retranslateUi(MainWindow);
        QObject::connect(limpiarButton, SIGNAL(clicked()), salidaEditText, SLOT(clear()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Simulaci\303\263n Estaci\303\263n de Servicio", 0));
#ifndef QT_NO_TOOLTIP
        MainWindow->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        surtidoresLabel->setText(QApplication::translate("MainWindow", "N\303\272mero de Surtidores", 0));
        empleadosLabel->setText(QApplication::translate("MainWindow", "N\303\272mero de Empleados", 0));
        label->setText(QApplication::translate("MainWindow", "Tiempo de Simulaci\303\263n (segundos)", 0));
        timeEdit->setDisplayFormat(QApplication::translate("MainWindow", "ss", 0));
        timeLabel->setText(QApplication::translate("MainWindow", "Tiempo Transcurrido", 0));
        comandoLabel->setText(QApplication::translate("MainWindow", "Comando", 0));
        iniciarButton->setText(QApplication::translate("MainWindow", "Iniciar Simulaci\303\263n", 0));
        limpiarButton->setText(QApplication::translate("MainWindow", "Limpiar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
