#include "AddBackupDialog.h"
#include "ui_AddBackupDialog.h"
#include "QMegaMessageBox.h"

#include <QFileDialog>

AddBackupDialog::AddBackupDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::AddBackupDialog),
    mSelectedFolder(),
    mMyBackupsFolder(),
    mSyncController(),
    mDeviceName()
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    mUi->setupUi(this);

    connect(&mSyncController, &SyncController::deviceName,
            this, &AddBackupDialog::onDeviceNameSet);

    mSyncController.getDeviceName();

#ifdef Q_OS_MACOS
    // Display our modal dialog embedded title label when parent is set
    mUi->embeddedTitleLabel->setVisible(this->parent() != nullptr);
#endif

    connect(mUi->addButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(mUi->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

AddBackupDialog::~AddBackupDialog()
{
    delete mUi;
}

void AddBackupDialog::setMyBackupsFolder(const QString& folder)
{
    mMyBackupsFolder = folder;
    mUi->backupToLabel->setText(mMyBackupsFolder + mDeviceName);
}

QString AddBackupDialog::getSelectedFolder()
{
    return mSelectedFolder;
}

void AddBackupDialog::on_changeButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),
                                                           QDir::home().path(),
                                                           QFileDialog::DontResolveSymlinks);
    if (!folderPath.isEmpty())
    {
        QString warningMessage;
        QString candidateDir (QDir::toNativeSeparators(QDir(folderPath).canonicalPath()));

        if (!mSyncController.isLocalFolderSyncable(candidateDir, mega::MegaSync::TYPE_BACKUP, warningMessage))
        {
            QMegaMessageBox::warning(nullptr, tr("Error"), warningMessage, QMessageBox::Ok);
        }
        else
        {
            mSelectedFolder = candidateDir;
            mUi->folderLineEdit->setText(folderPath);
            mUi->addButton->setEnabled(true);
        }
    }
}

void AddBackupDialog::onDeviceNameSet(const QString &devName)
{
    mDeviceName = devName;
    mUi->backupToLabel->setText(mMyBackupsFolder + mDeviceName);
}
