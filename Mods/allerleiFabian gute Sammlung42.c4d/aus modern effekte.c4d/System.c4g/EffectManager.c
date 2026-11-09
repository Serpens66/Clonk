/*-- Effektsystem --*/

#strict 3

static EFSM_CurrentData;
static EFSM_Init;
static EFSM_Level;

/* Effect-Konstanten */

static const EFSM_Blood = 1;		//1/0
static const EFSM_BulletCasing = 2;	//1/0
static const EFSM_BulletEffects = 3;	//0/1/2
static const EFSM_Fog = 4;		//0/1
static const EFSM_ExplosionEffects = 5;	//0/1/2
static const EFSM_Enlight = 6;		//0/1
static const EFSM_Darkness = 7;		//0/1
static const EFSM_Waratmosphere = 8;	//0/1
static const EFSM_Deco = 9;		//0/1


/* Globale Effektstufe setzen */

global func EFSM_SetEffects(int iLevel)
{
  iLevel ??= 0;
  if(!iLevel) iLevel = 3;
  EFSM_Init = true;
  EFSM_CurrentData = [];
  SetEffectData((iLevel ?? 0)>1, EFSM_Blood);
  SetEffectData((iLevel ?? 0)>2, EFSM_BulletCasing);
  SetEffectData((iLevel ?? 0)-1, EFSM_BulletEffects);
  SetEffectData((iLevel ?? 0)>1, EFSM_Fog);
  SetEffectData((iLevel ?? 0)-1, EFSM_ExplosionEffects);
  SetEffectData((iLevel ?? 0)>1, EFSM_Enlight);
  SetEffectData((iLevel ?? 0)>2, EFSM_Darkness);
  SetEffectData((iLevel ?? 0)>1, EFSM_Waratmosphere);
  SetEffectData((iLevel ?? 0)>1, EFSM_Deco);
  EFSM_Level = iLevel;
  return true;
}

global func GetEffectData(int iEffect)
{
  iEffect ??= 0;
  if(!EFSM_Init) EFSM_SetEffects();
  return EFSM_CurrentData[(iEffect ?? 0)];
}

global func SetEffectData(int iData, int iEffect)
{
  iData ??= 0;
  iEffect ??= 0;
  if(!EFSM_Init) EFSM_SetEffects();
  EFSM_Level = nil;
  EFSM_CurrentData[(iEffect ?? 0)] = iData;
  OnUpdateEffects(iEffect);
  return true;
}

global func OnUpdateEffects(int iEffect)
{
  //Dunkelheit aktualisieren
  iEffect ??= 0;
  if(S42MigrationEqual(iEffect, EFSM_Darkness) && !GetEffectData(EFSM_Darkness))
    {RemoveAll(DARK);}
  
  //Dekorationen entfernen/einfügen
  if(S42MigrationEqual(iEffect, EFSM_Deco) && !GetEffectData(EFSM_Deco))
  {
    for(var back in FindObjects(Find_Func("IsDeco")))
      RemoveObject(back);
  }
  if(S42MigrationEqual(iEffect, EFSM_Deco) && GetEffectData(EFSM_Deco) && !FindObject2(Find_Func("IsDeco")))
    GameCall("CreateDecoration");
}