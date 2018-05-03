#include "mnemonicdialog.h"
#include "ui_mnemonicdialog.h"
#include "rpc/server.h"
#include "rpc/client.h"
#include "wallet/mnemonic/mnemonic.h"
#include "wallet/db.h"
#include "wallet/wallet.h"
#include "init.h"
#include "util.h"
#include "guiutil.h"

#include <QApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QFileDialog>

MnemonicDialog::MnemonicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MnemonicDialog)
{
    ui->setupUi(this);
}

MnemonicDialog::~MnemonicDialog()
{
    delete ui;
}


void MnemonicDialog::on_importPrivatekey_clicked()
{
    QString privatekeystr = ui->privatekeyEdit->text();
    if(privatekeystr.isEmpty())
    {
        QMessageBox::critical(this, "Error", QString("Error: ") + QString::fromStdString("privatekey is null"));
        return;
    }
    privatekeystr.insert(0,QString("importprivkey "));
    ui->privatekeyEdit->clear();
    this->close();
    Q_EMIT cmdToConsole(privatekeystr);
}

void MnemonicDialog::on_importMnemonic_clicked()
{
    QString mnemonicstr = ui->mnemonicEdit->toPlainText();
    mnemonicstr.replace(QString(" "),QString("-"));
    mnemonicstr.insert(0,QString("importmnemonic "));
    if(mnemonicstr.isEmpty())
    {
        QMessageBox::critical(this, "Error", QString("Error: ") + QString::fromStdString("mnemonics is null"));
        return;
    }
    if(mnemonicstr.count(QRegExp("-")) < 11 || mnemonicstr.count(QRegExp("-")) > 24){
        QMessageBox::critical(this, "Error", QString("Error: ") + QString::fromStdString("input correct mnemonics"));
        return;
    }
    ui->mnemonicEdit->clear();
    this->close();
    Q_EMIT cmdToConsole(mnemonicstr);
    
}
void MnemonicDialog::on_importWallet_clicked()
{
    QString filepath = ui->fileEdit->text();
    if(filepath.isEmpty())
    {
        QMessageBox::critical(this, "Error", QString("Error: ") + QString::fromStdString("file path is null"));
        return;
    }
    filepath.insert(0,QString("importwallet "));
    ui->fileEdit->clear();
    this->close();
    Q_EMIT cmdToConsole(filepath);
}

void MnemonicDialog::on_fileButton_clicked()
{
    QString dataDir = GUIUtil::boostPathToQString(GetDataDir(false));
    QString dir = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Choose File"),dataDir));
    if(!dir.isEmpty())
        ui->fileEdit->setText(dir);        
}