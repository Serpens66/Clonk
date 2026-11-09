/*-- Axt --*/

#strict

local counter;

/* Aufschlag */
protected func Hit()
{
  Sound("MetalHit*");
}

func Initialize()
{
    counter=0;
}

protected func Activate(object pByObj)
{
	
	var ptree = FindObject2(Find_Func("IsTree"),Find_Distance(30),Find_Not(Find_Func("IsAxingDown")),Find_Not(Find_Func("IsStanding"))); // einen baum in der nähe finden, der noch nicht zerkleinert wird
    if(ptree)
    {
        ObjectSetAction(pByObj, "AxeDown",ptree); // starte das zerkleinern. siehe clonk skript. braucht action ExeDown.
        ptree->~AxedDown(pByObj); // im Baum die info vermerken, dass er gerade gehackt wird, damit nicht 2 clonks denselben baum hacken, denn das bringt zurzeit noch nichts
        // wichtig, wenn eingebuat wird, dass hacken abbrechbar ist, dann muss AxedDown nochmal aufgerufen werden, damits wieder 0 gesetzt wird und baum wieder gehackt werden kann
        // sicherheitshalber wird im Baumobjekt es aber auch wieder zurückgesetzt, wenn im ukreis kein Clonk mehr hackt.
    }    
    else return(_inherited());
}

func Axing() // Timercall, alle 8 sekunden (300Frames) ein aufruf
{
    if(Contained())
        if(Contained()->~IsClonk()) // wenn es von einme Clonk getragen wird
            if(GetAction(Contained()) eq "AxeDown") // wenn der Clonk gerade die aktion durchführt
            {
                var pClonk = Contained();
                var ptree = GetActionTarget(0,pClonk);
                if(ptree)
                {
                    var iWoodCount = ptree->GetComponent(WOOD);
                    counter++; // counter erhöhen
                    if (counter >= iWoodCount)    // x sekunden für jedes stueck holz
                    {
                        RemoveObject(ptree);
                        for(var i=0;i<iWoodCount;i++)
                            CreateObject (WOOD, 0, 0, -1); 
                        ObjectSetAction(pClonk, "Walk");
                        counter=0;
                        
                        if(FindObject2(Find_Func("IsTree"),Find_Distance(30),Find_Not(Find_Func("IsAxingDown")),Find_Not(Find_Func("IsStanding")))) return(Activate(pClonk)); // wenn noch ein Baum in direkter umgebung, dann weiterzerhacken
                    }
                }
                else //  wenn baum weg, aufhören
                    ObjectSetAction(pClonk, "Walk");
            }
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }


