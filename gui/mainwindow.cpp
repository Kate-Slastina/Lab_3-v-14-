#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QRegularExpressionValidator>
#include <QHeaderView>
#include <QColor>
#include <cmath>
#include <sstream>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRegularExpressionValidator *intValidator = new QRegularExpressionValidator(QRegularExpression("^-?\\d*$"), this);
    QRegularExpressionValidator *realValidator = new QRegularExpressionValidator(QRegularExpression("^-?\\d*\\.?\\d*$"), this);
    QRegularExpressionValidator *complexValidator = new QRegularExpressionValidator(
        QRegularExpression("^[\\d\\+\\-\\s\\.iI]*$"), this);
    ui->dataTextEdit->setValidator(complexValidator);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Число", "Тип числа"});
    ui->tableWidget_2->setColumnCount(2);
    ui->tableWidget_2->setHorizontalHeaderLabels({"ФИО студента", "Средний балл"});
    ui->tableWidget_3->setColumnCount(2);
    ui->tableWidget_3->setHorizontalHeaderLabels({"ФИО преподавателя", "Зарплата"});
    ui->tableWidget_6->setColumnCount(2);
    ui->tableWidget_6->setHorizontalHeaderLabels({"ФИО студента", "Средний балл"});

    for (auto table : {ui->tableWidget, ui->tableWidget_2, ui->tableWidget_3, ui->tableWidget_6}) {
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    ui->comboBox_tableSelector->addItem("Числа");
    ui->comboBox_tableSelector->addItem("Студенты (1)");
    ui->comboBox_tableSelector->addItem("Студенты (2)");
    ui->comboBox_tableSelector->addItem("Преподаватели");

    numbers = numbers + Complex(10,0) + Complex(3.14,0) + Complex(2,3);
    refreshNumbersTable();

    students2 = students2 + Student("Иванов Иван", 4.2) + Student("Петрова Мария", 4.8);
    refreshStudentsTable2();

    students6 = students6 + Student("Иванов Иван", 4.2);
    refreshStudentsTable6();

    teachers = teachers + Teacher("Сидоров Петр", 50000) + Teacher("Кузнецова Анна", 60000);
    refreshTeachersTable();

    updateCollectionInfo();

    connect(ui->comboBox_tableSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_tableSelector_currentIndexChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshNumbersTable()
{
    ui->tableWidget->setRowCount(0);
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        Complex c = *it;
        QString valueStr = QString::number(c.re) + (c.im >= 0 ? "+" : "") + QString::number(c.im) + "i";
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(valueStr));

        QString type;
        if (c.im == 0.0) {
            if (std::fmod(c.re, 1.0) == 0.0)
                type = "Целое число";
            else
                type = "Вещественное число";
        } else {
            type = "Комплексное число";
        }
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
    }
    updateCollectionInfo();
}

void MainWindow::refreshStudentsTable2()
{
    ui->tableWidget_2->setRowCount(0);
    for (auto it = students2.begin(); it != students2.end(); ++it) {
        int row = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row);
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(QString::fromStdString((*it).name)));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(QString::number((*it).grade)));
    }
    updateCollectionInfo();
}

void MainWindow::refreshStudentsTable6()
{
    ui->tableWidget_6->setRowCount(0);
    for (auto it = students6.begin(); it != students6.end(); ++it) {
        int row = ui->tableWidget_6->rowCount();
        ui->tableWidget_6->insertRow(row);
        ui->tableWidget_6->setItem(row, 0, new QTableWidgetItem(QString::fromStdString((*it).name)));
        ui->tableWidget_6->setItem(row, 1, new QTableWidgetItem(QString::number((*it).grade)));
    }
    updateCollectionInfo();
}

void MainWindow::refreshTeachersTable()
{
    ui->tableWidget_3->setRowCount(0);
    for (auto it = teachers.begin(); it != teachers.end(); ++it) {
        int row = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(row);
        ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(QString::fromStdString((*it).name)));
        ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(QString::number((*it).salary)));
    }
    updateCollectionInfo();
}

void MainWindow::updateCollectionInfo()
{
    QTableWidget* table = getCurrentTable();
    ui->listWidget_value->clear();
    if (!table) return;

    int rows = table->rowCount();
    ui->listWidget_value->addItem(QString("Количество элементов: %1").arg(rows));
    if (rows > 0) {
        ui->listWidget_value->addItem(QString("Первый элемент: %1").arg(getElementString(0, 0)));
        ui->listWidget_value->addItem(QString("Последний элемент: %1").arg(getElementString(rows-1, 0)));
    } else {
        ui->listWidget_value->addItem("Нет данных");
    }
}

QTableWidget* MainWindow::getCurrentTable() const
{
    int idx = ui->comboBox_tableSelector->currentIndex();
    switch (idx) {
        case 0: return ui->tableWidget;
        case 1: return ui->tableWidget_2;
        case 2: return ui->tableWidget_6;
        case 3: return ui->tableWidget_3;
        default: return ui->tableWidget;
    }
}

QString MainWindow::getElementString(int row, int col) const
{
    QTableWidget* table = getCurrentTable();
    if (!table) return QString();
    QTableWidgetItem* item = table->item(row, col);
    if (!item) return QString();
    QString val = item->text();
    int idx = ui->comboBox_tableSelector->currentIndex();
    if ((idx == 1 || idx == 2) && col == 0) {
        QTableWidgetItem* second = table->item(row, 1);
        if (second) val += " (" + second->text() + ")";
    } else if (idx == 3 && col == 0) {
        QTableWidgetItem* second = table->item(row, 1);
        if (second) val += " (" + second->text() + ")";
    }
    return val;
}

bool MainWindow::parseComplex(const QString &expr, double &re, double &im)
{
    QString s = expr;
    s.remove(' ');
    if (s.isEmpty()) return false;

    int pos = s.indexOf(QRegularExpression("[+-]"), 1);
    if (pos == -1) {
        bool ok;
        re = s.toDouble(&ok);
        if (!ok) return false;
        im = 0.0;
        return true;
    }

    QString reStr = s.left(pos);
    QString imStr = s.mid(pos);
    if (imStr.endsWith('i', Qt::CaseInsensitive))
        imStr.chop(1);
    if (imStr.isEmpty() || imStr == "+" || imStr == "-") {
        im = (imStr == "+") ? 1.0 : (imStr == "-") ? -1.0 : 0.0;
    } else {
        bool ok;
        im = imStr.toDouble(&ok);
        if (!ok) return false;
    }
    bool ok;
    re = reStr.toDouble(&ok);
    return ok;
}

Stream<long long> MainWindow::extractLongLongStream()
{
    auto seq = std::make_unique<MutableArraySequence<long long>>();
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        if ((*it).im == 0.0 && std::fmod((*it).re, 1.0) == 0.0) {
            seq->Append(static_cast<long long>((*it).re));
        }
    }
    return Stream<long long>(std::move(seq));
}

void MainWindow::on_pushButton_mapNumbers_clicked()
{
    numbers = numbers.Map<Complex>([](Complex c) { return Complex(c.re * 2, c.im * 2); });
    refreshNumbersTable();
    QMessageBox::information(this, "Map", "Все числа умножены на 2");
}

void MainWindow::on_pushButton_whereNumbers_clicked()
{
    numbers = numbers.Where([](Complex c) { return c.re > 10.0; });
    refreshNumbersTable();
    QMessageBox::information(this, "Where", "Оставлены числа с вещественной частью > 10");
}

void MainWindow::on_pushButton_reduceNumbers_clicked()
{
    double sum = 0.0;
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        sum += (*it).re;
    }
    QMessageBox::information(this, "Reduce", "Сумма вещественных частей: " + QString::number(sum));
}

void MainWindow::on_pushButton_addNumber_clicked()
{
    QString text = ui->dataTextEdit->text().trimmed();
    double re, im;
    if (parseComplex(text, re, im)) {
        numbers = numbers + Complex(re, im);
        refreshNumbersTable();
        ui->dataTextEdit->clear();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный формат числа");
    }
}

void MainWindow::on_pushButton_extractNumbers_clicked()
{
    QTableWidget* table = ui->tableWidget;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выделено ни одной строки");
        return;
    }
    QSet<int> rowsToKeep;
    for (const QTableWidgetSelectionRange &range : ranges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            rowsToKeep.insert(row);
        }
    }
    auto newSeq = std::make_unique<MutableArraySequence<Complex>>();
    int row = 0;
    for (auto it = numbers.begin(); it != numbers.end(); ++it, ++row) {
        if (rowsToKeep.contains(row)) {
            newSeq->Append(*it);
        }
    }
    numbers = Stream<Complex>(std::move(newSeq));
    refreshNumbersTable();
    QMessageBox::information(this, "Извлечение", "Оставлены только выделенные строки");
}

void MainWindow::on_pushButton_inversions_clicked()
{
    Stream<long long> ints = extractLongLongStream();
    if (ints.GetLength() == 0) {
        QMessageBox::information(this, "Инверсии", "Нет целых чисел для подсчёта инверсий");
        return;
    }
    size_t inv = CountInversions(ints);
    QMessageBox::information(this, "Инверсии", "Количество инверсий: " + QString::number(inv));
}

void MainWindow::on_pushButton_factorial_clicked()
{
    bool ok;
    int length = QInputDialog::getInt(this, "Факториалы", "Введите длину последовательности (1..20):", 5, 1, 20, 1, &ok);
    if (!ok) return;
    auto factStream = Recurrent::GenerateFactorialSequence(static_cast<size_t>(length));
    numbers = factStream.Map<Complex>([](long long v) { return Complex(static_cast<double>(v), 0); });
    refreshNumbersTable();
}

void MainWindow::on_pushButton_fibonacci_clicked()
{
    bool ok;
    int length = QInputDialog::getInt(this, "Числа Фибоначчи", "Введите длину последовательности (1..50):", 10, 1, 50, 1, &ok);
    if (!ok) return;
    auto fibStream = Recurrent::GenerateFibonacciSequence(static_cast<size_t>(length));
    numbers = fibStream.Map<Complex>([](long long v) { return Complex(static_cast<double>(v), 0); });
    refreshNumbersTable();
}

void MainWindow::on_pushButton_highlightNumbers_clicked()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *typeItem = ui->tableWidget->item(row, 1);
        if (!typeItem) continue;
        QString type = typeItem->text();
        QColor color;
        if (type == "Целое число")
            color = QColor(173, 216, 230);
        else if (type == "Вещественное число")
            color = QColor(152, 251, 152);
        else if (type == "Комплексное число")
            color = QColor(255, 182, 193);
        else continue;
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item) item->setBackground(color);
        }
    }
    QMessageBox::information(this, "Подсветка", "Цвета строк обновлены");
}

void MainWindow::on_pushButton_mapStudents2_clicked()
{
    students2 = students2.Map<Student>([](Student s) {
        s.grade = std::max(s.grade - 0.5, 2.0);
        return s;
    });
    refreshStudentsTable2();
    QMessageBox::information(this, "Map", "Всем студентам уменьшили балл на 0.5 (минимум 2)");
}

void MainWindow::on_pushButton_whereStudents2_clicked()
{
    students2 = students2.Where([](Student s) { return s.grade >= 3.0; });
    refreshStudentsTable2();
    QMessageBox::information(this, "Where", "Оставлены студенты с баллом >= 3");
}

void MainWindow::on_pushButton_reduceStudents2_clicked()
{
    double sum = 0.0;
    int count = 0;
    for (auto it = students2.begin(); it != students2.end(); ++it) {
        sum += (*it).grade;
        ++count;
    }
    double avg = (count > 0) ? sum / count : 0.0;
    QMessageBox::information(this, "Reduce", "Средний балл: " + QString::number(avg));
}

void MainWindow::on_pushButton_addStudent2_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Добавление студента", "Введите ФИО:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    double grade = QInputDialog::getDouble(this, "Добавление студента", "Введите средний балл (2.0..5.0):", 4.0, 2.0, 5.0, 1, &ok);
    if (!ok) return;
    students2 = students2 + Student(name.toStdString(), grade);
    refreshStudentsTable2();
}

void MainWindow::on_pushButton_extractStudents2_clicked()
{
    QTableWidget* table = ui->tableWidget_2;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выделено ни одной строки");
        return;
    }
    QSet<int> rowsToKeep;
    for (const QTableWidgetSelectionRange &range : ranges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) rowsToKeep.insert(row);
    }
    auto newSeq = std::make_unique<MutableArraySequence<Student>>();
    int row = 0;
    for (auto it = students2.begin(); it != students2.end(); ++it, ++row) {
        if (rowsToKeep.contains(row)) newSeq->Append(*it);
    }
    students2 = Stream<Student>(std::move(newSeq));
    refreshStudentsTable2();
    QMessageBox::information(this, "Извлечение", "Оставлены только выделенные строки");
}


void MainWindow::on_pushButton_mapStudents6_clicked()
{
    students6 = students6.Map<Student>([](Student s) {
        s.grade = std::max(s.grade - 0.5, 2.0);
        return s;
    });
    refreshStudentsTable6();
    QMessageBox::information(this, "Map", "Всем студентам уменьшили балл на 0.5 (минимум 2)");
}

void MainWindow::on_pushButton_whereStudents6_clicked()
{
    students6 = students6.Where([](Student s) { return s.grade >= 3.0; });
    refreshStudentsTable6();
    QMessageBox::information(this, "Where", "Оставлены студенты с баллом >= 3");
}

void MainWindow::on_pushButton_reduceStudents6_clicked()
{
    double sum = 0.0;
    int count = 0;
    for (auto it = students6.begin(); it != students6.end(); ++it) {
        sum += (*it).grade;
        ++count;
    }
    double avg = (count > 0) ? sum / count : 0.0;
    QMessageBox::information(this, "Reduce", "Средний балл: " + QString::number(avg));
}

void MainWindow::on_pushButton_addStudent6_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Добавление студента", "Введите ФИО:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    double grade = QInputDialog::getDouble(this, "Добавление студента", "Введите средний балл (2.0..5.0):", 4.0, 2.0, 5.0, 1, &ok);
    if (!ok) return;
    students6 = students6 + Student(name.toStdString(), grade);
    refreshStudentsTable6();
}

void MainWindow::on_pushButton_extractStudents6_clicked()
{
    QTableWidget* table = ui->tableWidget_6;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выделено ни одной строки");
        return;
    }
    QSet<int> rowsToKeep;
    for (const QTableWidgetSelectionRange &range : ranges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) rowsToKeep.insert(row);
    }
    auto newSeq = std::make_unique<MutableArraySequence<Student>>();
    int row = 0;
    for (auto it = students6.begin(); it != students6.end(); ++it, ++row) {
        if (rowsToKeep.contains(row)) newSeq->Append(*it);
    }
    students6 = Stream<Student>(std::move(newSeq));
    refreshStudentsTable6();
    QMessageBox::information(this, "Извлечение", "Оставлены только выделенные строки");
}

void MainWindow::on_pushButton_mapTeachers_clicked()
{
    teachers = teachers.Map<Teacher>([](Teacher t) {
        t.salary *= 1.1;
        return t;
    });
    refreshTeachersTable();
    QMessageBox::information(this, "Map", "Зарплата увеличена на 10%");
}

void MainWindow::on_pushButton_whereTeachers_clicked()
{
    teachers = teachers.Where([](Teacher t) { return t.salary <= 100000; });
    refreshTeachersTable();
    QMessageBox::information(this, "Where", "Оставлены преподаватели с зарплатой <= 100000");
}

void MainWindow::on_pushButton_reduceTeachers_clicked()
{
    double total = 0.0;
    for (auto it = teachers.begin(); it != teachers.end(); ++it) {
        total += (*it).salary;
    }
    QMessageBox::information(this, "Reduce", "Суммарная зарплата: " + QString::number(total));
}

void MainWindow::on_pushButton_addTeacher_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Добавление преподавателя", "Введите ФИО:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    double salary = QInputDialog::getDouble(this, "Добавление преподавателя", "Введите зарплату (0..1000000):", 50000, 0, 1000000, 1000, &ok);
    if (!ok) return;
    teachers = teachers + Teacher(name.toStdString(), salary);
    refreshTeachersTable();
}

void MainWindow::on_pushButton_extractTeachers_clicked()
{
    QTableWidget* table = ui->tableWidget_3;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выделено ни одной строки");
        return;
    }
    QSet<int> rowsToKeep;
    for (const QTableWidgetSelectionRange &range : ranges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) rowsToKeep.insert(row);
    }
    auto newSeq = std::make_unique<MutableArraySequence<Teacher>>();
    int row = 0;
    for (auto it = teachers.begin(); it != teachers.end(); ++it, ++row) {
        if (rowsToKeep.contains(row)) newSeq->Append(*it);
    }
    teachers = Stream<Teacher>(std::move(newSeq));
    refreshTeachersTable();
    QMessageBox::information(this, "Извлечение", "Оставлены только выделенные строки");
}

void MainWindow::on_pushButton_mergeStudents_clicked()
{
    students2 = students2.Merge(students6);
    refreshStudentsTable2();
    QMessageBox::information(this, "Слияние", "Данные из второй таблицы добавлены в первую");
}

void MainWindow::on_pushButton_compare_clicked()
{
    ui->listWidget_info->clear();
    if (students2.GetLength() != students6.GetLength()) {
        ui->listWidget_info->addItem(QString("Таблицы различаются: разное количество строк (%1 vs %2)")
                                     .arg(students2.GetLength()).arg(students6.GetLength()));
        return;
    }
    bool equal = true;
    auto it2 = students2.begin();
    auto it6 = students6.begin();
    int idx = 0;
    while (it2 != students2.end() && it6 != students6.end()) {
        const Student& s2 = *it2;
        const Student& s6 = *it6;
        if (!(s2.name == s6.name && std::abs(s2.grade - s6.grade) < 1e-9)) {
            equal = false;
            ui->listWidget_info->addItem(QString("Различие в строке %1: (%2, %3) vs (%4, %5)")
                                         .arg(idx+1)
                                         .arg(QString::fromStdString(s2.name)).arg(s2.grade)
                                         .arg(QString::fromStdString(s6.name)).arg(s6.grade));
        }
        ++it2; ++it6; ++idx;
    }
    if (equal) ui->listWidget_info->addItem("Таблицы идентичны");
    else ui->listWidget_info->insertItem(0, "Таблицы не равны");
}


void MainWindow::on_pushButton_getIndex_clicked()
{
    QTableWidget* table = getCurrentTable();
    if (!table || table->rowCount() == 0) {
        QMessageBox::warning(this, "Ошибка", "Таблица пуста");
        return;
    }
    int maxIdx = table->rowCount() - 1;
    bool ok;
    int idx = QInputDialog::getInt(this, "Доступ по индексу", QString("Введите индекс (0..%1):").arg(maxIdx), 0, 0, maxIdx, 1, &ok);
    if (!ok) return;
    QString value = getElementString(idx, 0);
    ui->listWidget_info->clear();
    ui->listWidget_info->addItem(QString("Индекс %1: %2").arg(idx).arg(value));
}

void MainWindow::on_comboBox_tableSelector_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateCollectionInfo();
    ui->listWidget_info->clear();
}