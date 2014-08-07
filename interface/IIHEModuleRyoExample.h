#ifndef UserCode_IIHETree_IIHEModuleRyoExample_h
#define UserCode_IIHETree_IIHEModuleRyoExample_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

// class decleration
class IIHEModuleRyoExample : public IIHEModule {
public:
  explicit IIHEModuleRyoExample(const edm::ParameterSet& iConfig);
  ~IIHEModuleRyoExample();
  
  void   pubBeginJob(){   beginJob() ; } ;
  void pubBeginEvent(){ beginEvent() ; } ;
  void   pubEndEvent(){   endEvent() ; } ;
  virtual void pubAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){ analyze(iEvent, iSetup) ; } ;
  
  virtual void beginEvent() ;
  virtual void endEvent() ;
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
};
#endif
