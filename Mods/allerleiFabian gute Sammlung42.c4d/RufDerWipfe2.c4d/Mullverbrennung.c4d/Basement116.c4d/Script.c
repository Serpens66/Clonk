/*-- Fundament --*/

#strict

#include BAS7

private func BasementID() { return(B116);}
private func BasementWidth(){ return(120);}
func IsBasement(){if(GetID()==B116)return(1);return(0);}  

//Fenster-Kruscht 
private func AddWindowLight(idDef,iX,iY,iPhase)
{
}

public func AddWindowLightEx(idDef,iX,iY,iPhase)
{
}
