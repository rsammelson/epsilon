#ifndef APPS_CONTAINER_H
#define APPS_CONTAINER_H

#include "home/app.h"
#include "on_boarding/app.h"
#include "hardware_test/app.h"
#include "usb/app.h"
#include "apps_window.h"
#include "empty_battery_window.h"
#include "math_toolbox.h"
#include "variable_box_controller.h"
#include "exam_pop_up_controller.h"
#include "exam_pop_up_controller_delegate.h"
#include "battery_timer.h"
#include "suspend_timer.h"
#include "backlight_dimming_timer.h"
#include "shared/global_context.h"

#ifdef EPSILON_BOOT_PROMPT
#include "on_boarding/pop_up_controller.h"
#endif

#include <ion/events.h>

class AppsContainer : public Container, ExamPopUpControllerDelegate, Ion::StorageDelegate {
public:
  AppsContainer();
  static bool poincareCircuitBreaker();
  virtual int numberOfApps() = 0;
  virtual App::Snapshot * appSnapshotAtIndex(int index) = 0;
  App::Snapshot * hardwareTestAppSnapshot();
  App::Snapshot * onBoardingAppSnapshot();
  App::Snapshot * usbConnectedAppSnapshot();
  void reset();
  Poincare::Context * globalContext();
  MathToolbox * mathToolbox();
  VariableBoxController * variableBoxController();
  void suspend(bool checkIfPowerKeyReleased = false);
  virtual bool dispatchEvent(Ion::Events::Event event) override;
  bool switchTo(App::Snapshot * snapshot) override;
  void run() override;
  bool updateBatteryState();
  void refreshPreferences();
  void displayExamModePopUp(bool activate);
  void shutdownDueToLowBattery();
  void setShiftAlphaStatus(Ion::Events::ShiftAlphaStatus newStatus);
#ifdef EPSILON_BOOT_PROMPT
  OnBoarding::PopUpController * promptController();
#endif
  void redrawWindow();
  // Exam pop-up controller delegate
  void examDeactivatingPopUpIsDismissed() override;
  // Ion::StorageDelegate
  void storageDidChangeForRecord(const Ion::Storage::Record record) override;
  void storageIsFull() override;
protected:
  Home::App::Snapshot * homeAppSnapshot() { return &m_homeSnapshot; }
private:
  Window * window() override;
  int numberOfContainerTimers() override;
  Timer * containerTimerAtIndex(int i) override;
  bool processEvent(Ion::Events::Event event);
  void resetShiftAlphaStatus();
  bool updateAlphaLock();

  AppsWindow m_window;
  EmptyBatteryWindow m_emptyBatteryWindow;
  Shared::GlobalContext m_globalContext;
  MathToolbox m_mathToolbox;
  VariableBoxController m_variableBoxController;
  ExamPopUpController m_examPopUpController;
#ifdef EPSILON_BOOT_PROMPT
  OnBoarding::PopUpController m_promptController;
#endif
  BatteryTimer m_batteryTimer;
  SuspendTimer m_suspendTimer;
  BacklightDimmingTimer m_backlightDimmingTimer;
  Home::App::Snapshot m_homeSnapshot;
  OnBoarding::App::Snapshot m_onBoardingSnapshot;
  HardwareTest::App::Snapshot m_hardwareTestSnapshot;
  USB::App::Snapshot m_usbConnectedSnapshot;
};

#endif
