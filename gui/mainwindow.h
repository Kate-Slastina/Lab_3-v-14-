#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "Stream.hpp"
#include "Complex.hpp"
#include "Student.hpp"
#include "Teacher.hpp"
#include "RecurrentSequences.hpp"
#include "Inversions.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Для таблицы чисел (Complex)
    void on_pushButton_mapNumbers_clicked();
    void on_pushButton_whereNumbers_clicked();
    void on_pushButton_reduceNumbers_clicked();
    void on_pushButton_addNumber_clicked();
    void on_pushButton_extractNumbers_clicked();
    void on_pushButton_inversions_clicked();        // инверсии для целых чисел
    void on_pushButton_factorial_clicked();
    void on_pushButton_fibonacci_clicked();

    // Для студентов (таблица 2)
    void on_pushButton_mapStudents2_clicked();
    void on_pushButton_whereStudents2_clicked();
    void on_pushButton_reduceStudents2_clicked();
    void on_pushButton_addStudent2_clicked();
    void on_pushButton_extractStudents2_clicked();
    void on_pushButton_mergeStudents_clicked();     // слияние таблиц 2 и 6

    // Для студентов (таблица 6)
    void on_pushButton_mapStudents6_clicked();
    void on_pushButton_whereStudents6_clicked();
    void on_pushButton_reduceStudents6_clicked();
    void on_pushButton_addStudent6_clicked();
    void on_pushButton_extractStudents6_clicked();

    // Для преподавателей
    void on_pushButton_mapTeachers_clicked();
    void on_pushButton_whereTeachers_clicked();
    void on_pushButton_reduceTeachers_clicked();
    void on_pushButton_addTeacher_clicked();
    void on_pushButton_extractTeachers_clicked();

    void on_pushButton_compare_clicked();           // сравнить таблицы студентов 2 и 6
    void on_pushButton_getIndex_clicked();          // доступ по индексу в текущей таблице
    void on_comboBox_tableSelector_currentIndexChanged(int index);
    void on_pushButton_highlightNumbers_clicked();  // подсветка чисел по типу

private:
    Ui::MainWindow *ui;

    // Три независимых потока
    Stream<Complex> numbers;            // комплексные числа (для простоты)
    Stream<Student> students2;
    Stream<Student> students6;
    Stream<Teacher> teachers;

    // Вспомогательные методы
    void refreshNumbersTable();
    void refreshStudentsTable2();
    void refreshStudentsTable6();
    void refreshTeachersTable();
    void updateCollectionInfo();
    QTableWidget* getCurrentTable() const;
    QString getElementString(int row, int col) const;
    bool parseComplex(const QString &expr, double &re, double &im);
    Stream<long long> extractLongLongStream();     // для подсчёта инверсий
};

#endif // MAINWINDOW_H