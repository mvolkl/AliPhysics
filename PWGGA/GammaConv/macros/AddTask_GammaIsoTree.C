void AddTask_GammaIsoTree(
  Int_t     isMC                          = 0,
  Int_t     IsHeavyIon                    = 0,
  TString   TaskEventCutnumber           = "00010113",
  TString   photonCutNumberV0Reader      = "00200009327000008250400000",
  TString   TaskClusterCutnumberEMC      = "1111100010022700000",
  TString   TaskClusterCutnumberPHOS     = "1111100010022700000",
  TString   TaskConvCutnumber            = "0dm0000922700000dge0404000",
  TString   periodNameV0Reader            = "",
  Bool_t    kHistograms                   = kTRUE,
  TString   corrTaskSetting = "",
  Int_t     enableExtMatchAndQA           = 0,
  Bool_t    enableTriggerOverlapRej       = kTRUE,
  TString   settingMaxFacPtHard           = "3.",       // maximum factor between hardest jet and ptHard generated
  Bool_t    makeAdditionalHistos          = kFALSE,
  Bool_t    storeTracks                   = kTRUE,
  Bool_t    storeEMCalCluster             = kTRUE,
  Bool_t    storePHOSCluster              = kTRUE,
  Bool_t    storeConversions              = kTRUE,
  Bool_t    doIsolation                   = kTRUE,
  Bool_t    doTrackMatching               = kFALSE,
  ){
  

  // ================== GetAnalysisManager ===============================
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    return ;
  }

  // ================== GetInputEventHandler =============================
  AliVEventHandler *inputHandler=mgr->GetInputEventHandler();

  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

//=========  Set Cutnumber for V0Reader ================================
  TString cutnumberPhoton     = photonCutNumberV0Reader.Data();
  TString cutnumberEvent      = "00000003";
  if(IsHeavyIon==1)
    cutnumberEvent = "10000003";
  else if(IsHeavyIon==2)
    cutnumberEvent = "80000003";

//========= Check V0 Reader in  ANALYSIS manager  =====
  TString V0ReaderName        = Form("V0ReaderV1_%s_%s",cutnumberEvent.Data(),cutnumberPhoton.Data());
  AliV0ReaderV1 *fV0ReaderV1  =  NULL;
  if( !(AliV0ReaderV1*)mgr->GetTask(V0ReaderName.Data()) ){
    cout << "V0Reader: " << V0ReaderName.Data() << " not found!!"<< endl;
    return;
  } else {
    cout << "V0Reader: " << V0ReaderName.Data() << " found!!"<< endl;
  }

  AliAnalysisTaskGammaIsoTree *fQA = new AliAnalysisTaskGammaIsoTree("GammaIsoTree");
  
  fQA->SetV0ReaderName(V0ReaderName);
  fQA->SetIsMC(isMC);
  fQA->SetYCutMC(0.9);
  
  // fQA->SetSaveClusterCells(doSaveClusterCells);
  // fQA->SetSaveEventProperties(doSaveEventProp);
  // fQA->SetDoAdditionalHistos(makeAdditionalHistos);

  TObjArray *rmaxFacPtHardSetting = settingMaxFacPtHard.Tokenize("_");
  if(rmaxFacPtHardSetting->GetEntries()<1){cout << "ERROR: AddTask_ClusterQA during parsing of settingMaxFacPtHard String '" << settingMaxFacPtHard.Data() << "'" << endl; return;}
  Bool_t fMinPtHardSet        = kFALSE;
  Double_t minFacPtHard       = -1;
  Bool_t fMaxPtHardSet        = kFALSE;
  Double_t maxFacPtHard       = 100;
  Bool_t fSingleMaxPtHardSet  = kFALSE;
  Double_t maxFacPtHardSingle = 100;
  for(Int_t i = 0; i<rmaxFacPtHardSetting->GetEntries() ; i++){
    TObjString* tempObjStrPtHardSetting     = (TObjString*) rmaxFacPtHardSetting->At(i);
    TString strTempSetting                  = tempObjStrPtHardSetting->GetString();
    if(strTempSetting.BeginsWith("MINPTHFAC:")){
      strTempSetting.Replace(0,10,"");
      minFacPtHard               = strTempSetting.Atof();
      cout << "running with min pT hard jet fraction of: " << minFacPtHard << endl;
      fMinPtHardSet        = kTRUE;
    } else if(strTempSetting.BeginsWith("MAXPTHFAC:")){
      strTempSetting.Replace(0,10,"");
      maxFacPtHard               = strTempSetting.Atof();
      cout << "running with max pT hard jet fraction of: " << maxFacPtHard << endl;
      fMaxPtHardSet        = kTRUE;
    } else if(strTempSetting.BeginsWith("MAXPTHFACSINGLE:")){
      strTempSetting.Replace(0,16,"");
      maxFacPtHardSingle         = strTempSetting.Atof();
      cout << "running with max single particle pT hard fraction of: " << maxFacPtHardSingle << endl;
      fSingleMaxPtHardSet        = kTRUE;
    } else if(rmaxFacPtHardSetting->GetEntries()==1 && strTempSetting.Atof()>0){
      maxFacPtHard               = strTempSetting.Atof();
      cout << "running with max pT hard jet fraction of: " << maxFacPtHard << endl;
      fMaxPtHardSet        = kTRUE;
    }
  }
  // Create Event Cuts
  AliConvEventCuts *analysisEventCuts = new AliConvEventCuts();
  analysisEventCuts->SetV0ReaderName(V0ReaderName);

  analysisEventCuts->SetTriggerOverlapRejecion(enableTriggerOverlapRej);
  if(fMinPtHardSet)
    analysisEventCuts->SetMinFacPtHard(minFacPtHard);
  if(fMaxPtHardSet)
    analysisEventCuts->SetMaxFacPtHard(maxFacPtHard);
  if(fSingleMaxPtHardSet)
    analysisEventCuts->SetMaxFacPtHardSingleParticle(maxFacPtHardSingle);
  analysisEventCuts->SetCorrectionTaskSetting(corrTaskSetting);
  if (periodNameV0Reader.CompareTo("") != 0) analysisEventCuts->SetPeriodEnum(periodNameV0Reader);
  analysisEventCuts->InitializeCutsFromCutString(TaskEventCutnumber.Data());
  analysisEventCuts->SetFillCutHistograms("",kFALSE);

  // EMC cluster cuts
  AliCaloPhotonCuts *analysisClusterCutsEMC = new AliCaloPhotonCuts(isMC,"analysisClusterCutsEMC","analysisClusterCutsEMC");
  analysisClusterCutsEMC->SetV0ReaderName(V0ReaderName);
  analysisClusterCutsEMC->SetCorrectionTaskSetting(corrTaskSetting);
  // analysisClusterCutsEMC->SetCaloTrackMatcherName(TrackMatcherNameEMC);
  analysisClusterCutsEMC->SetExtendedMatchAndQA(enableExtMatchAndQA);
  analysisClusterCutsEMC->InitializeCutsFromCutString(TaskClusterCutnumberEMC.Data());
  analysisClusterCutsEMC->SetFillCutHistograms("");

  // PHOS cluster cuts
  AliCaloPhotonCuts *analysisClusterCutsPHOS = new AliCaloPhotonCuts(isMC,"analysisClusterCutsPHOS","analysisClusterCutsPHOS");
  analysisClusterCutsPHOS->SetV0ReaderName(V0ReaderName);
  // analysisClusterCutsPHOS->SetCaloTrackMatcherName(TrackMatcherNamePHOS);
  analysisClusterCutsPHOS->SetExtendedMatchAndQA(enableExtMatchAndQA);
  analysisClusterCutsPHOS->InitializeCutsFromCutString(TaskClusterCutnumberPHOS.Data());
  analysisClusterCutsPHOS->SetFillCutHistograms("");
  
  AliConversionPhotonCuts* analysisConvCuts = new AliConversionPhotonCuts();
  analysisConvCuts->SetV0ReaderName(V0ReaderName);
  analysisConvCuts->InitializeCutsFromCutString(TaskConvCutnumber.Data());
  analysisConvCuts->SetFillCutHistograms("");
  
  fQA->SetEventCuts(analysisEventCuts,IsHeavyIon);
  fQA->SetClusterCutsEMC(analysisClusterCutsEMC,IsHeavyIon);
  fQA->SetClusterCutsPHOS(analysisClusterCutsPHOS,IsHeavyIon);
  fQA->SetConvCuts(analysisConvCuts,IsHeavyIon);
  fQA->SetDoTrackIso(kTRUE);
  fQA->SetTrackIsoR(0.4);
  fQA->SetCorrectionTaskSetting(corrTaskSetting);
  fQA->SetSaveConversions(storeConversions);
  fQA->SetSaveEMCClusters(storeEMCalCluster);
  fQA->SetSavePHOSClusters(storePHOSCluster);
  fQA->SetSaveTracks(storeTracks);
  
  mgr->AddTask(fQA);

  mgr->ConnectInput(fQA, 0,  cinput );
  AliAnalysisDataContainer *coutput = mgr->CreateContainer( "GammaIsoTree",
                                                            TTree::Class(),
                                                            AliAnalysisManager::kOutputContainer,
                                                            "GammaIsoTree.root");
  AliAnalysisDataContainer *histos= mgr->CreateContainer( "GammaIsoTree_histos",
                                                            TList::Class(),
                                                            AliAnalysisManager::kOutputContainer,
                                                            "GammaIsoTree_histos.root");
  mgr->ConnectOutput (fQA, 1, histos );
  mgr->ConnectOutput (fQA, 2, coutput );

  // mgr->ConnectOutput (fQA, 2, "GammaIsoTreeHistos", TList::Class(), AliAnalysisManager::kOutputContainer, Form("%s:TreeAnalysisHistos", mgr->GetCommonFileName())) );
  return;
}
