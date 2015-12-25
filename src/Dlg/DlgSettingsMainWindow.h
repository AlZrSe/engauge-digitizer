#ifndef DLG_SETTINGS_MAIN_WINDOW_H
#define DLG_SETTINGS_MAIN_WINDOW_H

#include "DlgSettingsAbstractBase.h"

class QComboBox;
class QGridLayout;
class QPushButton;
class QSpinBox;
class MainWindowModel;

/// Dialog for editing main window settings, which are entirely independent of all documents
class DlgSettingsMainWindow : public DlgSettingsAbstractBase
{
  Q_OBJECT;

public:
  /// Single constructor.
  DlgSettingsMainWindow(MainWindow &mainWindow);
  virtual ~DlgSettingsMainWindow();

  virtual void createOptionalSaveDefault (QHBoxLayout *layout);
  virtual QWidget *createSubPanel ();
  virtual void load (CmdMediator &cmdMediator); // Not used. Use loadMainWindowModel instead

  /// Replaced load method since the main window settings are independent of document, unlike other DlgSettings* classes
  void loadMainWindowModel (CmdMediator &cmdMediator,
                            const MainWindowModel &modelMainWindow);

private slots:
  void slotZoomControl (const QString);

protected:
  virtual void handleOk ();

private:

  void createControls (QGridLayout *layout,
                       int &row);
  void updateControls();

  QComboBox *m_cmbZoomControl;

  MainWindowModel *m_modelMainWindowBefore;
  MainWindowModel *m_modelMainWindowAfter;
};

#endif // DLG_SETTINGS_MAIN_WINDOW_H