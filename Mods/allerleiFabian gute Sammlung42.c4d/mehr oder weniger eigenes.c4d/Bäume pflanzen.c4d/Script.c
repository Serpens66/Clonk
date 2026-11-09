#strict


func Initialize()
{
}


func Seed()
{
var obj;
for(var obj in FindObjects(Find_OCF(OCF_Chop)))
{obj->Reproduction();obj->Reproduction();};
}
