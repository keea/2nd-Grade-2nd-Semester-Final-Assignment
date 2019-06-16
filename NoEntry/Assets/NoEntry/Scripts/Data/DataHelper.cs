using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;

public class DataHelper 
{
    public static T[] LoadData<T>(string fileName){
       string jsonString = File.ReadAllText (Application.dataPath + "/Data/"+fileName+".json");
       T[] data = JsonHelper.FromJson <T>(jsonString);
       return data;
   }

    public static T LoadData<T>(string fileName, int index){
       string jsonString = File.ReadAllText (Application.dataPath + "/Data/"+fileName+".json");
       T[] data = JsonHelper.FromJson<T>(jsonString);
       return data[index];
   }

    //하나만 바꾸기.
   public static void ChangeData<T>(string fileName, int idx, T data){
       string jsonString = File.ReadAllText (Application.dataPath + "/Data/"+fileName+".json");
       T[] saveData = JsonHelper.FromJson <T>(jsonString);
       saveData[idx] = data;

       string toJson = JsonHelper.ToJson(saveData, prettyPrint:true);
       Debug.Log(toJson);
       File.WriteAllText(Application.dataPath + "/Data/"+fileName+".json",toJson);
   }

     //통째로 바꾸기.
   public static void ChangeData<T>(string fileName, T[] data){
       string toJson = JsonHelper.ToJson(data, prettyPrint:true);
       Debug.Log(toJson);
       File.WriteAllText(Application.dataPath + "/Data/"+fileName+".json",toJson);
   }

}
