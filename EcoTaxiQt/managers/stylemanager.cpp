#include "stylemanager.h"

styleManager::styleManager(QApplication *app)
{
    QVariantMap vars;
    vars["mainColor"] = "#007700";
    vars["mainHoverColor"] = "#005500";
    vars["backgroundColor"] = "#eeeeee";
    vars["backgroundHoverColor"] = "#aaaaaa";
    vars["textColor"] = "#222222";
    vars["grayColor"] = "#b9b9b9";

    vars["btnFontSize"] = "27px";
    vars["textFontSize"] = "20px";
    vars["editFontSize"] = "20px";

    vars["bgBorderRadius"] = "30px";
    vars["btnBorderRadius"] = "26px";
    vars["editBorderRadius"] = "18px";

    QString styles = R"A(
* {
    font-family: Inter Black;
}

/* #################################################### MAIN STYLES #################################################### */

QWidget {
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QLabel {
    background-color: #00000000;
    color: )A" +
                     vars["mainColor"].toString() + R"A(;
    font-size: )A" +
                     vars["textFontSize"].toString() + R"A(;
}

QLabel[color="white"] {
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

.QFrame {
    background-color: transparent;
}

.QFrame#greenBack, QFrame[color="green"] {
    background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["bgBorderRadius"].toString() + R"A(;
}

/* #################################################### BUTTONS STYLES #################################################### */

QPushButton {
    background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    color: white;
    font: 900 )A" +
                     vars["btnFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["btnBorderRadius"].toString() + R"A(;
    padding: 6px;
}

QPushButton:hover {
    background-color: )A" +
                     vars["mainHoverColor"].toString() + R"A(;
}

QPushButton:pressed {
    background-color: )A" +
                     vars["textColor"].toString() + R"A(;
}

QPushButton:flat {
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["mainColor"].toString() + R"A(;
}

QPushButton:flat:hover {
    background-color: )A" +
                     vars["backgroundHoverColor"].toString() + R"A(;
    color: )A" +
                     vars["textColor"].toString() + R"A(;
}

QPushButton:flat:pressed {
    background-color: )A" +
                     vars["textColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QPushButton:disabled {
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QPushButton[color="gray"] {
    background-color: )A" +
                     vars["grayColor"].toString() + R"A(;
    color: )A" +
                     vars["mainColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 4px;
}

/* #################################################### EDITS STYLES #################################################### */

QLineEdit {
    background-color:)A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    font-size: )A" +
                     vars["editFontSize"].toString() + R"A(;
}

QLineEdit[color="red"] {
    background-color: red;
}

QTextEdit {
    background-color:)A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    font-size: )A" +
                     vars["editFontSize"].toString() + R"A(;
}

/* #################################################### LABELS STYLES #################################################### */

QLabel#Header {
    font: 900 )A" +
                     vars["btnFontSize"].toString() + R"A(;
}

QLabel#EcoTaxi {
    font: 900 80px;
}

QLabel#EcoTaxiWhite {
    font: 900 50px;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QLabel {
    font-weight: 900;
}

QLabel[color="white"] {
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QLabel#warningLabel {
    color: red;
}

/* #################################################### COMBOBOX STYLES #################################################### */

QComboBox {
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    font: bold )A" +
                     vars["editFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
}

QComboBox:disabled {
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QComboBox::drop-down:button {
    background-color: #00000000;
    width: )A" +
                     vars["editFontSize"].toString() + R"A(;
}

QComboBox::item {
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    padding: 5px;
}

QComboBox::down-arrow {
    width: 0;
    height: 0;
    border-left: 15px solid )A" +
                     vars["backgroundColor"].toString() + R"A(;
    border-right: 15px solid )A" +
                     vars["backgroundColor"].toString() + R"A(;
    right: 10px;
    border-top: 15px solid )A" +
                     vars["textColor"].toString() + R"A(;
}

QComboBox::down-arrow:on {
    border-top: none;
    border-bottom: 15px solid )A" +
                     vars["textColor"].toString() + R"A(;
}

QListView {
    padding: 5px;
    outline: 0;
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    border: none;
}

QListView::item {
    color: )A" +
                     vars["textColor"].toString() + R"A(;
    border-radius: 5px;
    padding: 5px;
    border: none;
}

QListView::item:selected {
    color: )A" +
                     vars["mainColor"].toString() + R"A(;
    border: none;
}

/* #################################################### NAVIGATION COMBOBOX STYLES #################################################### */

QComboBox[type="nav"] {
    background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    font: bold )A" +
                     vars["btnFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["btnBorderRadius"].toString() + R"A(;
    padding: 0 20px;
}

QComboBox[type="nav"]:disabled {
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QComboBox[type="nav"]::drop-down:button {
    background-color: #00000000;
}

QComboBox[type="nav"]::down-arrow {
    border: none;
}

QComboBox[type="nav"]::down-arrow:on {
    border: none;
}

QComboBox[type="nav"] QListView {
    outline: 0;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["btnBorderRadius"].toString() + R"A(;
    border: none;
}

QComboBox[type="nav"] QListView::item {
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    border: none;
}

QComboBox[type="nav"] QListView::item:selected {
    color: )A" +
                     vars["mainColor"].toString() + R"A(;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    border: none;
}

/* #################################################### TABLE STYLES #################################################### */

QTableView {
    font-family: Inter;
    font-size: 20px;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    gridline-color: #b9b9b9;
    color: )A" +
            vars["textColor"].toString() + R"A(;
    border: 1px solid #b9b9b9;
}

QHeaderView::section {
    font-family: Inter;
    color: )A" +
            vars["textColor"].toString() + R"A(;
    background-color: #b9b9b9;
    font-size: 20px;
    padding: 4px;
    text-align: center;
    border: 1px solid )A" +
                      vars["backgroundColor"].toString() + R"A(;
    font-weight: bold;
}

QTableView::item:selected {
    background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

/* #################################################### SCROLLBAR STYLES #################################################### */

QScrollBar:horizontal {
    height: 15px;
    background: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QScrollBar::handle:horizontal {
    min-width: 20px;
    background: )A" +
                     vars["mainColor"].toString() + R"A(;
}

QScrollBar:vertical {
    width: 15px;
    background: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QScrollBar::handle:vertical {
    min-height: 20px;
    background: )A" +
                     vars["mainColor"].toString() + R"A(;
}

/* #################################################### CALENDAR STYLES #################################################### */

QToolButton {
    background-color: )A" +
            vars["mainColor"].toString() + R"A(;
}

QCalendarWidget QToolButton {
    height: 60px;
    width: 180px;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    font-size: 24px;
    icon-size: 64px;
    background-color: white;
    color: )A" + vars["mainColor"].toString() + R"A(;
    border: none;
}
QCalendarWidget QWidget#qt_calendar_navigationbar {
    background-color: white; /* Background color for the navigation bar */
}
QCalendarWidget QMenu {
    width: 150px;
    left: 20px;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    font-size: )A" +
                vars["textFontSize"].toString() + R"A(;
}
QCalendarWidget QSpinBox {
    width: 150px;
    font-size: 24px;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    selection-background-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    selection-color: )A" +
                     vars["textColor"].toString() + R"A(;
}

QCalendarWidget QSpinBox::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width:65px;
}
QCalendarWidget QSpinBox::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width:65px;
}
QCalendarWidget QSpinBox::up-arrow {
    width:56px;
    height:56px;
}
QCalendarWidget QSpinBox::down-arrow {
    width:56px;
    height:56px;
}

/* header row */
QCalendarWidget QWidget {
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    alternate-background-color: )A" +
                                vars["mainColor"].toString() + R"A(;
}

/* normal days */
QCalendarWidget QAbstractItemView:enabled
{
    font-size:24px;
    color: )A" +
            vars["textColor"].toString() + R"A(;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
    selection-background-color: )A" +
                                vars["backgroundColor"].toString() + R"A(;
    selection-color: )A" +
                     vars["mainColor"].toString() + R"A(;
    selection-border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
}

/* days in other months */
/* navigation bar */
QCalendarWidget QWidget#qt_calendar_navigationbar
{
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    background-color: )A" +
                     vars["backgroundColor"].toString() + R"A(;
}

QCalendarWidget QAbstractItemView:disabled
{
    color: )A" +
            vars["grayColor"].toString() + R"A(;
}

/* #################################################### CONTEXT MENU STYLES #################################################### */

QMenu::item
{
    color: )A" +
            vars["textColor"].toString() + R"A(;
}

QMenu
{
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
}

/* #################################################### TIME EDIT STYLES #################################################### */

QTimeEdit
{
    background-color: )A" +
                      vars["grayColor"].toString() + R"A(;
    border: none;
    color: )A" +
            vars["mainColor"].toString() + R"A(;
    font-size: )A" +
               vars["btnFontSize"].toString() + R"A(;
    border-radius: )A" +
                     vars["editBorderRadius"].toString() + R"A(;
    border-width: 4px;
    spacing: 5px;
}

QTimeEdit:disabled
{
    color: )A" +
            vars["backgroundColor"].toString() + R"A(;
}

QTimeEdit::up-button {
    subcontrol-origin: border;
    subcontrol-position: top right;
    width: 40px;
    background-color: #00000000;
}
QTimeEdit::down-button {
    subcontrol-origin: border;
    subcontrol-position: bottom right;
    width: 40px;
    background-color: #00000000;
}
QTimeEdit::up-arrow {
    width: 0;
    height: 0;
    border-left: 15px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-right: 15px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-bottom: 15px solid )A" +
                     vars["mainColor"].toString() + R"A(;
}
QTimeEdit::down-arrow {
    width: 0;
    height: 0;
    border-left: 15px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-right: 15px solid )A" +
                     vars["grayColor"].toString() + R"A(;
    border-top: 15px solid )A" +
                     vars["mainColor"].toString() + R"A(;
}

QTimeEdit::up-arrow:disabled
{
    border: none;
}
QTimeEdit::down-arrow:disabled
{
    border: none;
}

QDateTimeEdit {
    background-color: )A" + vars["backgroundColor"].toString() + R"A(;
    color: )A" + vars["textColor"].toString() + R"A(;
    border-radius: )A" + vars["editBorderRadius"].toString() + R"A(;
    padding: 5px;
    font-size: )A" + vars["editFontSize"].toString() + R"A(;
}

QDateTimeEdit::drop-down {
    background-color: )A" + vars["backgroundColor"].toString() + R"A(;
    border: none;
}

QDateTimeEdit::down-arrow {
    width: 0;
    height: 0;
}

QDateTimeEdit::up-arrow {
    width: 0;
    height: 0;
}

QDateTimeEdit::up-button, QDateTimeEdit::down-button {
    subcontrol-origin: border;
    background-color: #00000000;
    width: 20px;
}

QDateTimeEdit:disabled {
    background-color: )A" + vars["backgroundColor"].toString() + R"A(;
    color: )A" + vars["grayColor"].toString() + R"A(;
}


/* #################################################### CHECK BOX STYLES #################################################### */

QCheckBox
{
    background-color: transparent;
    font-size: )A" +
                vars["editFontSize"].toString() + R"A(;
    color: )A" +
            vars["textColor"].toString() + R"A(;
}

QCheckBox:checked
{
    color: )A" +
            vars["mainColor"].toString() + R"A(;
}

QCheckBox::indicator
{
    width: 20px;
    height: 20px;
    background-color: )A" +
                    vars["backgroundColor"].toString() + R"A(;
    border-style: solid;
    border-width: 2px;
    border-color: )A" +
                    vars["mainColor"].toString() + R"A(;
}

QCheckBox::indicator:checked
{
    background-color: )A" +
                    vars["mainColor"].toString() + R"A(;
}

QCheckBox[color="white"]
{
    color: )A" +
            vars["backgroundColor"].toString() + R"A(;
}

QCheckBox:checked[color="white"]
{
    color: )A" +
            vars["backgroundColor"].toString() + R"A(;
}

QCheckBox::indicator[color="white"]
{
    width: 20px;
    height: 20px;
    background-color: )A" +
                    vars["mainColor"].toString() + R"A(;
    border-style: solid;
    border-width: 2px;
    border-color: )A" +
                    vars["backgroundColor"].toString() + R"A(;
}

QCheckBox::indicator:checked[color="white"]
{
    background-color: )A" +
                    vars["backgroundColor"].toString() + R"A(;
}

)A";

    app->setStyleSheet(styles);
}
