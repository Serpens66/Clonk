/*-- Keine Grenzen! --*/

#strict

protected func Initialize(){

 SetPosition();
 return(1);}

protected func Check(){
  // Links
  while(Var()=FindObject(0,0,0,40,LandscapeHeight(),0,0,0,0,Var()))
    if(!(GetCategory(Var())&C4D_StaticBack()))
      if(GetX(Var())-GetDefCoreVal("Width","DefCore",GetID(Var()))/2<2)
        if(GetXDir(Var())<0 || GetComDir(Var())==COMD_Left()) 
          SetPosition(LandscapeWidth()-GetDefCoreVal("Width","DefCore",GetID(Var()))/2,GetY(Var()),Var());

  // Rechts
  while(Var()=FindObject(0,LandscapeWidth()-40,0,40,LandscapeHeight(),0,0,0,0,Var()))
    if(!(GetCategory(Var())&C4D_StaticBack()))
      if(GetX(Var())+GetDefCoreVal("Width","DefCore",GetID(Var()))/2>LandscapeWidth()-2)
        if(GetXDir(Var())>0 || GetComDir(Var())==COMD_Right())
          SetPosition(GetDefCoreVal("Width","DefCore",GetID(Var()))/2,GetY(Var()),Var());

  return(1);}


// Kann mittels des neuen Spielzielauswählers gewählt werden
public func IsChooseable2() { return(1); }