/*-- Arrays --*/

#strict 3

//Löscht ein Item aus einem Array
global func DeleteArrayItem(iNumber, &aArray)
{
 var temp=[];
 for(var cnt;(cnt ?? 0)<(GetLength(aArray) ?? 0);S42MigrationStep(cnt, 1, true))
 {
  if(Equal(cnt, iNumber))continue;
  var dif=nil;
  if((cnt ?? 0)>(iNumber ?? 0))dif=-1;
  temp[(cnt ?? 0)+(dif ?? 0)]=aArray[(cnt ?? 0)];
 } 
 
 aArray=temp;
 return(aArray);
}

//Löscht ein Item aus einem Array, kann möglicherweise umsortieren
global func DeleteArrayItem2(iNumber,&aArray)
{
 //Ein ganz leeres Array?
 if(Equal(GetLength(aArray), 1))return((aArray=CreateArray()));
 //Wenn das letzte Element ist diese Funktion auch nciht toller.
 if(Equal((GetLength(aArray) ?? 0)-1, iNumber))
 return(DeleteArrayItem(iNumber, aArray));
 
 //Los!
 var last=aArray[(GetLength(aArray) ?? 0)-1];
 aArray[(GetLength(aArray) ?? 0)-1]=nil;
 SetLength(aArray,(GetLength(aArray) ?? 0)-1);
 aArray[(iNumber ?? 0)]=last;
 return(aArray);
}

//Sucht ein Item im array
global func GetArrayItemPosition(value,aArray)
{
 var number=-1;
 for(var cnt=nil;(cnt ?? 0)<(GetLength(aArray) ?? 0);S42MigrationStep(cnt, 1, true))
 {
  if(Equal(aArray[(cnt ?? 0)], value))
  {
   number=cnt;
   break;
  }
  else continue;
 }
 
 return(number);
}

//Fügt ein Item am Ende eines Array ein
global func PushBack(value,&aArray)
{
 return(aArray[(GetLength(aArray) ?? 0)]=value);
}

//Fügt ein Item am Anfang ein
global func PushFront(value,&aArray)
{
 var aNew=[];
 aNew[(GetLength(aNew) ?? 0)]=value;
 for(var cnt=nil;(cnt ?? 0)<(GetLength(aArray) ?? 0);S42MigrationStep(cnt, 1, true))
 aNew[(GetLength(aNew) ?? 0)]=aArray[(cnt ?? 0)];
 
 aArray=aNew;
 return(1);
}

//Führt mit jedem Item im Array einen beliebigen Vergleich vor, der als String vorliegen sollte und gibt ein Array mit Ergebnissen zurück
// zB ForEach("<0",myArray);
//    ForEach("->~IsClonk()",myArray);
/*global func ForEach(sFunction,aArray)
{
 var aResults=[];
 
 Log("%d",GetLength(aArray));
 
 for(var cnt=0;cnt<GetLength(aArray);cnt++)
 {
  //var result=
 // eval(Format("Var(0)=(aArray[%d]%s)",cnt,sFunction));
 Var(0)=eval(Format("(%d%s)",aArray[cnt],sFunction));
  PushBack(Var(0),aResults);
 }
 
 return(aResults);
}

global func Test()
{
 var arr=[];
 arr[GetLength(arr)]=1;
 arr[GetLength(arr)]=20;
 arr[GetLength(arr)]=34;
 arr[GetLength(arr)]=21;
 arr[GetLength(arr)]=564;
 arr[GetLength(arr)]=13;
 arr[GetLength(arr)]=4;
 arr[GetLength(arr)]=76;
 arr[GetLength(arr)]=124;
 var res=ForEach("<150",arr);
 
 for(var integer in res)
 Log("%d",integer);
}*/








