/*--- Bausatz ---*/

#strict 3


public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
   // Fahrstuhl suchen
   // var f = FindObject2(Find_Func("IsElevator"),Find_InRect(-20,-100,40,200),Find_Not(Find_ID(ELC2))); // IsElevator beinhaltet auch groeßere fahrstuhle und seilban, daher einfach direkt die IDs nehmen
   var f = FindObject2(Find_InRect(-20,-100,40,200),Find_Or(Find_ID(ELEC),Find_ID(ELE_),Find_ID(CPEC)));
   if(f)
   {
       var wall = f->~Elevator(); // gebäude
       if(wall)
       {
           if(!wall->hassperre()) // wenn der Fahrstuhl noch keine Sperre hat
           {
               f->Halt(); // anhalten
               wall->addsperre();
               if(Equal(GetID(f), CPEC))
                 CreateObject(SSPE,((GetX(f) ?? 0)-4)-(GetX() ?? 0),((GetY(wall) ?? 0)+44)-(GetY() ?? 0),GetOwner(pClonk));
               else
                 CreateObject(SSPE,((GetX(f) ?? 0)-4)-(GetX() ?? 0),((GetY(wall) ?? 0)+36)-(GetY() ?? 0),GetOwner(pClonk));
               // CreateObject(SSPE,(GetX(f)-4)-GetX(),(GetY(f)+20)-GetY(),GetOwner(pClonk));
               RemoveObject();
           }
       }
   }
// var f=FindObject(ELEC,-20,-20,40,40);
// if( f == 0)
// {
// f=FindObject(CPEC,-20,-100,40,200);
// var wall =FindObject(CPEL,-20,-20,40,40);
// } 
// if( f == 0)
// {
// f=FindObject(CPEC,-20,-100,40,200);
// var wall =FindObject(ELE_,-20,-20,40,40);
// } 
// if(f != 0)
// {
// f->Halt();
// if(wall != 0)
    // {
        // if(!wall->hassperre())
            // {
            // wall->addsperre();
            // CreateObject(SSPE,(GetX(f)-4)-GetX(),(GetY(wall)+44)-GetY(),GetOwner(pClonk));
            // }
    // }
    // else // wenn kein fahrstuhlgebäude auffindbar
        // {
        // CreateObject(SSPE,(GetX(f)-4)-GetX(),(GetY(f)+20)-GetY(),GetOwner(pClonk));
        // }
        // RemoveObject();
// }	
return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
