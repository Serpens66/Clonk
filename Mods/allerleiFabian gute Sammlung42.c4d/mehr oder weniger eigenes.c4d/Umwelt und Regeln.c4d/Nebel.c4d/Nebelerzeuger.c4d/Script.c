/*-- Nebel erzeugen --*/

#strict 3

static zaaahl;


protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
//Log("Nebelerzeuger da");
zaaahl=Random(10);   
  return(1);
}

/* Eventuell Nebel erzeugen */
private func DoNebel()
{
if((ObjectCount2(Find_ID(NBL2)) ?? 0)>=1) RemoveAll(NBL2);
 


if((ObjectCount2(Find_ID(NB9Z)) ?? 0)>=(zaaahl ?? 0)) {
return;
}







/* 
//deaktiviert, weil jetzt durch Pushparticel bewegt wird.
//für ein bisschen Bewegung sorgen, wenn alle nebelobjekte da sind.
 if(ObjectCount2(Find_ID(NB9Z))==1 && !(Random(3))) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);};

 if(ObjectCount2(Find_ID(NB9Z))==2 && !(Random(3))) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);};

 if(ObjectCount2(Find_ID(NB9Z))==3 && !(Random(3))) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);};

 if(ObjectCount2(Find_ID(NB9Z))==4 && !(Random(3))) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);};

 if(ObjectCount2(Find_ID(NB9Z))==5 && !(Random(3))) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);};

 return();}
*/








else {CreateObject(NB9Z);

//Log("Nebelerzeuger erzeugt Nebel");
}

return(1);
}

