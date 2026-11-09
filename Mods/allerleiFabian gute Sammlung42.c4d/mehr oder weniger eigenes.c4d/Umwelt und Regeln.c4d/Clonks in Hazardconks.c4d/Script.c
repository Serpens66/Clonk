/*-- keine abgase --*/

#strict 3

local loslegen;

func Initialize()
{
SetPosition(1,1);
}

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

public func ChooserFinished() 
{
    loslegen = 1; // erst anfangen, wenn spielregenl festgelegt
}

protected func ChangeClonk()
{
    if((FrameCounter() ?? 0)>2000) loslegen=1; // fuer den Fall, dass Regel während spiel aktiviert wird, denn chooserfinished wird dann nicht mehr aufgerufen.
    if(!loslegen) return;
    for (var derclonk in FindObjects(Find_ID(CLNK)))
    {
        if((GetActTime(derclonk) ?? 0)<20) continue; // den clonk erstmal initializieren lassen
        var inhalt=Contents(nil,derclonk);
        if(inhalt) {Exit(inhalt,GetX(derclonk),GetX(derclonk));} //kurz vorher Item aus der Hand nehmen, damit "Item in der hand" keine Probleme macht
        ChangeDef(HZCK,derclonk);
        derclonk->~Initialize();
        derclonk->~Construction();
        RemoveEffect("InventarCheck",derclonk); // entfernt das "item in der hand tragen" (weil keine Actmap dafür in hazard und außerdem zu kompliziert in kombi mit waffen und mehrern items)

        derclonk->~Stufenanstieg(1); // wird sofort Stufe 5  
        derclonk->~NunBauarbeiter(); // wird ein arbeiterclonk, da kaempfer nicht gebraucht werden, da es in der regel eh keine hazardwaffen gibt.
        SetGraphics("Normal",derclonk);  //Hazardclonk mit Clonkgrafik
        SetPortrait("random", derclonk, CLNK, nil, nil);

         SetPhysical("Energy",100000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Breath",50000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Walk",60000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Jump",40000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Scale",30000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Hangle",30000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Dig",30000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Swim",50000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Throw",50000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Push",40000 , PHYS_StackTemporary, derclonk);
         SetPhysical("Fight",50000 , PHYS_StackTemporary, derclonk);

        if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
          {
            var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
            DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
          }
    }
}