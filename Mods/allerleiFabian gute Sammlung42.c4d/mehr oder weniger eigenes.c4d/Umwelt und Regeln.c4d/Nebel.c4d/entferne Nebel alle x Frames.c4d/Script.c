/*-- Nebelentferner alle x Frames --*/

#strict 3

static zaaahl;

func Initialize()
{
SetPosition();
}

func KillNebel()
{
if (!(Random(3))) {
CreateObject(NBL2);Log("Nebelentferner erstellt, nebel sollte nun entfernt werden");
if ((ObjectCount2(Find_ID(NBL1)) ?? 0)>=1) RemoveAll(NBL1);  //nebelerzeuger entfernen, da dieser sonst evtl den entferner entfernt
}
return(1);
}
