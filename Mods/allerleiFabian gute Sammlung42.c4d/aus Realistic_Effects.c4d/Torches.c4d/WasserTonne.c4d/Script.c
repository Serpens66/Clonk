/*-- Wasser Tonne --*/

#strict

// im clonk hitze skript, wird dann nach solchen Tonnen ausschau gehalten

protected func Initialize() 
{
SetPlrViewRange(FogViewRange(),0,1);  
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    return(30); 
}

protected func Completion()
{
    SetCategory(C4D_Vehicle()); // vehicle reicht, ohne foreground und ist dennoch noch abreißbar wegen construction
    return();
}





  