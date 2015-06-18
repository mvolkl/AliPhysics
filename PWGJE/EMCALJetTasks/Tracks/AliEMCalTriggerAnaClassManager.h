/**
 * \file AliEMCalTriggerAnaClassManager.h
 * \brief Declaration of a management class for trigger classes
 *
 * \author Markus Fasel <markus.fasel@cern.ch>, Lawrence Berkeley National Laboratory
 */
#ifndef ALIEMCALTRIGGERANACLASSMANAGER_H
#define ALIEMCALTRIGGERANACLASSMANAGER_H
/* Copyright(c) 1998-2015, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

#include <TNamed.h>

class TObjArray;

/**
 * \namespace EMCalTriggerPtAnalysis
 * \brief Analysis of high-p_{t} tracks in triggered events
 *
 * This namespace contains classes for the analysis of high-p_{t} tracks in
 * triggered events.
 */
namespace EMCalTriggerPtAnalysis {

class AliEMCalTriggerAnaTriggerClass;
class AliEMCalTriggerAnaTriggerDecision;
class AliEMCalTriggerEventData;

/**
 * \class AliEMCalTriggerAnaClassManager
 * \brief Manager for trigger classes
 *
 * This class manages trigger classes, meaning it serves as a
 * container and steers the event selection.
 */
class AliEMCalTriggerAnaClassManager: public TNamed {
public:
  AliEMCalTriggerAnaClassManager();
  AliEMCalTriggerAnaClassManager(const char *name);
  virtual ~AliEMCalTriggerAnaClassManager();

  void PerformEventSelection(AliEMCalTriggerEventData *trgevent);

  void AddTriggerClass(AliEMCalTriggerAnaTriggerClass *triggerclass);
  void SetTriggerDecision(AliEMCalTriggerAnaTriggerDecision *triggerdecision);

  /**
   * Get the list of selected trigger classes. Event selection has to be performed before.
   * \return The list of selected trigger classes.
   */
  TObjArray * GetSelectedTriggerClasses() const { return fSelected; }
  /**
   * Get list of all trigger classes
   * \return List of all trigger classes
   */
  TObjArray * GetAllTriggerClasses() const { return fTriggerClasses; }

  bool HasMinBiasTrigger() const;

private:
  TObjArray         *fTriggerClasses;               ///< List of trigger classes
  TObjArray         *fSelected;                     ///< List of selected trigger classes

  /// \cond CLASSIMP
  ClassDef(AliEMCalTriggerAnaClassManager, 1);
  /// \endcond
};

} /* namespace EMCalTriggerPtAnalysis */

#endif /* ALIEMCALTRIGGERANACLASSMANAGER_H */
