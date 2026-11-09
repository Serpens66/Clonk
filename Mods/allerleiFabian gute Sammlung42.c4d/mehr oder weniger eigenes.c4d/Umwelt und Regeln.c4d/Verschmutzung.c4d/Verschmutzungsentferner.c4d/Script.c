/*-- Verschmutzungsentferner --*/

#strict 3

func Initialize()
{
SetPosition();
}

func KillVerschmutzung()
{
if ((ObjectCount2(Find_ID(VMG1)) ?? 0)>=1) RemoveAll(VMG1);

if (Equal(ObjectCount2(Find_ID(VM9Z)), nil))
{RemoveAll(VMG2);
return;};

if (Equal(ObjectCount2(Find_ID(VM9Z)), 5))
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
return;};

if (Equal(ObjectCount2(Find_ID(VM9Z)), 4))
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
return;};

if (Equal(ObjectCount2(Find_ID(VM9Z)), 3))
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
CreateObject(VM9Z);
return;};

if (Equal(ObjectCount2(Find_ID(VM9Z)), 2))
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
CreateObject(VM9Z);
return;};

if (Equal(ObjectCount2(Find_ID(VM9Z)), 1))
{ClearParticles("Nebel2");
ResetGamma();
RemoveAll(VM9Z);
RemoveAll(VMG2);
return;};
}
