/*-- Verbrannter Müll --*/

#strict

local wert;

public func IsWaste(){ return(wert);}

func Initialize()
{
//Log("VerbrannterMüllObjekt gestartet");
SetPosition(5,5);
wert=0;
}

// *2 weil das Verbr.Müll Objekt festhängt und der Wert demnach halbiert wird... -> dieses objekt als ausnahme in umweltschutz definiert, ist also egal ob feststeckt oder nicht-> am ende wieder durch 2 teilen, oder alle *2 entfernen 

public func Verbrannt(ID,wasteamount)
{
//Log(Format("ID aufruf gestartet: %i, amount: %v", ID, wasteamount));
// zuerst das verbrennen von müll
if(ID==SHLK || ID==_SKA) wert=wasteamount*2*4;    
if(ID==CHMU || ID==_CHF) wert=wasteamount*2*4;
if(ID==MTWS) wert=wasteamount*2*4;
if(ID==HMWS || ID==_MSK) wert=wasteamount*2*4;
if(ID==CORP) wert=wasteamount*2*4;
if(ID==YECK) wert=wasteamount*2*4;

if(ID==SNSS) wert=wasteamount*1500*2; // 1*... explodierte Atombombe
if(ID==ATO2) wert=wasteamount*2*1000; // explodierter Atomtorpedo

// wenn keine abgase regel eingestellt, ist hier schluss. sofern wert noch nicht anders gesetzt, ist es also 0
if(FindObject(NOWS)) return();

                                    // noch Metallschmelzgebäude hinzufügen?              


// aktive Industriegebäude
if(ID==CHEM) wert=wasteamount*2;
if(ID==WRKS) wert=wasteamount*2*2;
if(ID==ANVL) wert=wasteamount*2;
if(ID==RSRC) wert=wasteamount*2; //20
if(ID==POWR) wert=wasteamount*2; //20 alle 2 Verbrennungen
if(ID==CLE2) wert=wasteamount*2; // Heizkraftwerk noch hinzufügen
if(ID==C_F2) wert=wasteamount*2*2; //Raffinerie 20
if(ID==C_F1) wert=wasteamount*2*2; // Chemiefabrik 20
if(ID==C_F3) wert=wasteamount*10*2; //Raumschifffabrik 10*10=100
if(ID==SREA) wert=wasteamount*2*2;  // Atomkraftwerkreaktor 50*2=100
if(ID==SOF1) wert=wasteamount*2*2;    //Chemielabor 10*2=20
if(ID==SOF3) wert=wasteamount*2*5;  //Maschinenfabrik 10*5=50
if(ID==SOF2) wert=wasteamount*2*2;   //Waffenfabrik 10*2=20

if(!wert) wert = wasteamount*2;   // alles andere
// steckt nicht mehr fest, also durch 2 teilen, ums rückgängig zu machen
wert = wert/2;

}


func Verraucht()
{
//Log("Verraucht");
wert=0;
RemoveObject();
return();
}