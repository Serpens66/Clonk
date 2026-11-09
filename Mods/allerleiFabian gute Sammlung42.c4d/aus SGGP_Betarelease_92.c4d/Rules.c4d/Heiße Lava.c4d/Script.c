#strict 3

local obj;
local aArray;

public func Initialize()
{
  aArray = CreateArray(29);
  aArray[0] = PUD_;
  aArray[1] = STGT;
  aArray[2] = DHD_;
  aArray[3] = DUMM;
  aArray[4] = DUM2;
  aArray[5] = WAHL;
  aArray[6] = SGR1;
  aArray[7] = SGR2;
  
  aArray[8] = RIG1;
  aArray[9] = RIG2;
  aArray[10] = RIG3;
  aArray[11] = TEL1;
  aArray[12] = TEL2;
  aArray[13] = TEL3;
  aArray[14] = TELZ;
  aArray[15] = EPOD;
  aArray[16] = TELK;
  aArray[17] = TEL4;
  aArray[18] = TELS;
  aArray[19] = TELO;
  
  aArray[20] = AIRL;
  aArray[21] = BAIL;
  
  aArray[22] = CONS;
  aArray[23] = CNS2;
  aArray[24] = SSLS;
  aArray[25] = HNG1;
  aArray[26] = HNG2;
  
  aArray[27] = PUDD;
  aArray[28] = CURS;
  aArray[29] = KULL;
  
  aArray[30] = TASL;
  aArray[31] = HNG2;
  aArray[32] = TWR2;
  aArray[33] = GATL;
  aArray[34] = GATR;
  aArray[35] = SUB1;
  aArray[36] = LNKT;
  aArray[37] = PUMP;
  aArray[37] = HLAV;
  
  SetPosition(nil,nil);
  return(1);
}

public func Check()
{
  for(obj in FindObjects(Find_Not(Find_Action("Throw")),Find_NoContainer()))
  {
   if(Or(Equal(GetMaterial(AbsX(GetX(obj)),AbsX(GetY(obj))), Material("DuroLava")), Equal(GetMaterial(AbsX(GetX(obj)),AbsX(GetY(obj))), Material("Lava"))))
   {
    if(FindInArray(GetID(obj),aArray))
    {
     return(1);
    }
    var help;
    help = ((GetDefCoreVal("Width","DefCore",GetID(obj)) ?? 0) + (GetDefCoreVal("Height","DefCore",GetID(obj)) ?? 0))/2;
    obj->CastPXS("FlyAshes",help,50);
    obj->RemoveObject();
   }
  }
}