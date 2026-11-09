/*-- Verbrannter Müll --*/

#strict 3

local wert;

public func IsWaste(){ return(wert);}

func Initialize()
{
//Log("VerbrannterMüllObjekt gestartet");
SetPosition(5,5);
wert=nil;
}

// *2 weil das Verbr.Müll Objekt festhängt und der Wert demnach halbiert wird... -> dieses objekt als ausnahme in umweltschutz definiert, ist also egal ob feststeckt oder nicht-> am ende wieder durch 2 teilen, oder alle *2 entfernen 

public func Verbrannt(ID,wasteamount)
{
//Log(Format("ID aufruf gestartet: %i, amount: %v", ID, wasteamount));
// zuerst das verbrennen von müll
if(Or(Equal(ID, SHLK), Equal(ID, _SKA))) wert=(wasteamount ?? 0)*2*4;
if(Or(Equal(ID, CHMU), Equal(ID, _CHF))) wert=(wasteamount ?? 0)*2*4;
if(Equal(ID, MTWS)) wert=(wasteamount ?? 0)*2*4;
if(Or(Equal(ID, HMWS), Equal(ID, _MSK))) wert=(wasteamount ?? 0)*2*4;
if(Equal(ID, CORP)) wert=(wasteamount ?? 0)*2*4;
if(Equal(ID, YECK)) wert=(wasteamount ?? 0)*2*4;

if(Equal(ID, SNSS)) wert=(wasteamount ?? 0)*1500*2; // 1*... explodierte Atombombe
if(Equal(ID, ATO2)) wert=(wasteamount ?? 0)*2*1000; // explodierter Atomtorpedo

// wenn keine abgase regel eingestellt, ist hier schluss. sofern wert noch nicht anders gesetzt, ist es also 0
if(FindObject(NOWS)) return;

                                    // noch Metallschmelzgebäude hinzufügen?              


// aktive Industriegebäude
if(Equal(ID, CHEM)) wert=(wasteamount ?? 0)*2;
if(Equal(ID, WRKS)) wert=(wasteamount ?? 0)*2*2;
if(Equal(ID, ANVL)) wert=(wasteamount ?? 0)*2;
if(Equal(ID, RSRC)) wert=(wasteamount ?? 0)*2; //20
if(Equal(ID, POWR)) wert=(wasteamount ?? 0)*2; //20 alle 2 Verbrennungen
if(Equal(ID, CLE2)) wert=(wasteamount ?? 0)*2; // Heizkraftwerk noch hinzufügen
if(Equal(ID, C_F2)) wert=(wasteamount ?? 0)*2*2; //Raffinerie 20
if(Equal(ID, C_F1)) wert=(wasteamount ?? 0)*2*2; // Chemiefabrik 20
if(Equal(ID, C_F3)) wert=(wasteamount ?? 0)*10*2; //Raumschifffabrik 10*10=100
if(Equal(ID, SREA)) wert=(wasteamount ?? 0)*2*2;  // Atomkraftwerkreaktor 50*2=100
if(Equal(ID, SOF1)) wert=(wasteamount ?? 0)*2*2;    //Chemielabor 10*2=20
if(Equal(ID, SOF3)) wert=(wasteamount ?? 0)*2*5;  //Maschinenfabrik 10*5=50
if(Equal(ID, SOF2)) wert=(wasteamount ?? 0)*2*2;   //Waffenfabrik 10*2=20

if(!wert) wert = (wasteamount ?? 0)*2;   // alles andere
// steckt nicht mehr fest, also durch 2 teilen, ums rückgängig zu machen
wert = (wert ?? 0)/2;

}


func Verraucht()
{
//Log("Verraucht");
wert=nil;
RemoveObject();
return;
}