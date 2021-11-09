#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>

//Define some variables that deal with the display and iniitial button values
double calcValue = 0.0;
bool divideTrigger = false;
bool multiplyTrigger = false;
bool addTrigger = false;
bool subtractTrigger = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set the display text to calcValue
    //And connect all number buttons to the corresponding slot function

    ui->display->setText(QString::number(calcValue));
    QPushButton *numButtons[10];
    for (int i = 0; i < 10; ++i)
    {
        QString buttonName = "Button" + QString::number(i);
        numButtons[i] = MainWindow::findChild<QPushButton *>(buttonName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    //Connect all function buttons to the appropriate slot functions
    connect(ui->ButtonPlus, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->ButtonMinus, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->ButtonMult, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->ButtonDivide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->ButtonEquals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
    connect(ui->ButtonSign, SIGNAL(released()), this, SLOT(SignButtonPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Define all the slot functions for the buttons here

//Handle when a number button is pressed

void MainWindow::NumPressed()
{
    QPushButton *button = (QPushButton *)sender();
    QString buttonValue = button->text();
    QString displayValue = ui->display->text();
    if ((displayValue.toDouble() == 0) || (displayValue.toDouble() == 0.0))
    {
        ui->display->setText(buttonValue);
    }
    else
    {
        QString newValue = displayValue + buttonValue;
        double doubleNewValue = newValue.toDouble();
        ui->display->setText(QString::number(doubleNewValue, 'g', 16));
    }
}

//Handle when one of the four math function keys are pressed

void MainWindow::MathButtonPressed()
{

    //Reset all triggers to false
    divideTrigger = false;
    multiplyTrigger = false;
    addTrigger = false;
    subtractTrigger = false;

    //Get the value currently displayed and of button pressed
    QString displayValue = ui->display->text();
    calcValue = displayValue.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString buttonValue = button->text();

    //If the button value corresponds to a given function,
    //set that math function's trigger to true
    if (QString::compare(buttonValue, "/", Qt::CaseInsensitive) == 0)
    {
        divideTrigger = true;
    }
    else if (QString::compare(buttonValue, "*", Qt::CaseInsensitive) == 0)
    {
        multiplyTrigger = true;
    }
    else if (QString::compare(buttonValue, "+", Qt::CaseInsensitive) == 0)
    {
        addTrigger = true;
    }
    else if (QString::compare(buttonValue, "-", Qt::CaseInsensitive) == 0)
    {
        subtractTrigger = true;
    }

    //Clear display so the user can input another number
    ui->display->setText("");
}

//Handle when the equal button is pressed

void MainWindow::EqualButtonPressed()
{

    //Set variables for solution and currently displayed value
    double solution = 0.0;
    QString displayValue = ui->display->text();
    double doubleDisplayValue = displayValue.toDouble();

    //Depending on which trigger has been set to true,
    //the appropriate function will be done to the current
    //calculated value
    if (addTrigger || subtractTrigger || multiplyTrigger || divideTrigger)
    {
        if (addTrigger)
        {
            solution = calcValue + doubleDisplayValue;
        }
        else if (subtractTrigger)
        {
            solution = calcValue - doubleDisplayValue;
        }
        else if (multiplyTrigger)
        {
            solution = calcValue * doubleDisplayValue;
        }
        else if (divideTrigger)
        {
            solution = calcValue / doubleDisplayValue;
        }
    }

    //Update the display text to the solution
    ui->display->setText(QString::number(solution));
}

//Handle when the sign button is pressed

void MainWindow::SignButtonPressed()
{

    //Create variables with currently displayed value,
    //a regular expression, and a regular expression match
    QString displayValue = ui->display->text();
    QRegularExpression re("[-]?[0-9.]*", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(displayValue);

    //If the display value matches the given regular expression
    //then multiply the display value by -1 and update the display text.
    if (match.hasMatch())
    {
        double doubleDisplayValue = displayValue.toDouble();
        double doubleDisplayValueSign = -1 * doubleDisplayValue;
        ui->display->setText(QString::number(doubleDisplayValueSign));
    }
}
