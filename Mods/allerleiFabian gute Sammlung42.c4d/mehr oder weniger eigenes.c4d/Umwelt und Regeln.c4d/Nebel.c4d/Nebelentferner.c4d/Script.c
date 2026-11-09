/*-- Nebelentferner --*/

#strict


func Initialize()
{
SetPosition();
}

func KillNebel()
{
if (ObjectCount2(Find_ID(NBL1))>=1) RemoveAll(NBL1);

if (ObjectCount2(Find_ID(NB9Z))==0) 
{if (ObjectCount2(Find_ID(NBL4))>=1) {RemoveAll(NBL4);} return();}

if (ObjectCount2(Find_ID(NB9Z))==6) //sollten eig maximal 5 sein
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return();};

if (ObjectCount2(Find_ID(NB9Z))==5) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return();};

if (ObjectCount2(Find_ID(NB9Z))==4) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return();};

if (ObjectCount2(Find_ID(NB9Z))==3)
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return();};

if (ObjectCount2(Find_ID(NB9Z))==2) 
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
return();};

if (ObjectCount2(Find_ID(NB9Z))==1)
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
return();};
}

