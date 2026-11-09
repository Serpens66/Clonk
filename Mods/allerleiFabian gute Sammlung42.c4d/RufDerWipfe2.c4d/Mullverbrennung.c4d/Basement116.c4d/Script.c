/*-- Fundament --*/

#strict 3

#include BAS7

private func BasementID() { return(B116);}
private func BasementWidth(){ return(120);}
func IsBasement(){if(Equal(GetID(), B116))return(1);return(nil);}

//Fenster-Kruscht 
private func AddWindowLight(idDef,iX,iY,iPhase)
{
}

public func AddWindowLightEx(idDef,iX,iY,iPhase)
{
}
