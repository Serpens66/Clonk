/*-- Verschmutzungsentferner --*/

#strict

func Initialize()
{
SetPosition();
}

func KillVerschmutzung()
{
if (ObjectCount2(Find_ID(VMG1))>=1) RemoveAll(VMG1);

if (ObjectCount2(Find_ID(VM9Z))==0) 
{RemoveAll(VMG2);
return();};

if (ObjectCount2(Find_ID(VM9Z))==5) 
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
return();};

if (ObjectCount2(Find_ID(VM9Z))==4) 
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
return();};

if (ObjectCount2(Find_ID(VM9Z))==3) 
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
return();};

if (ObjectCount2(Find_ID(VM9Z))==2) 
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
return();};

if (ObjectCount2(Find_ID(VM9Z))==1) 
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
RemoveAll(VMG2);
return();};
}
