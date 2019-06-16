using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TutorialManager : MonoBehaviour
{
   public GameObject[] popUps;
   public GameObject skipButton;
   private int popUpIndex = 0;

   public float waitTime = 1f;
   bool isSuccess = false;

   bool isTutoEnd = false;

   void Update()
   {
      if(popUpIndex >= popUps.Length)
         popUpIndex = popUps.Length-1;
      
      for(int i=0; i<popUps.Length; i++)
      {
         if(i == popUpIndex)
         {
            popUps[i].SetActive(true);
         }else
         {
            popUps[i].SetActive(false);
         }
      }

      if(popUpIndex == popUps.Length-1){
         GameObject.Find("Monster").GetComponent<MonsterScripts>().isTuto = false;
         skipButton.SetActive(false);
         if(!isTutoEnd){
            GameObject.Find("GameManager").GetComponent<GameManager>().isTimeInit = true;
            isTutoEnd = true;
         }
      }

      if(isSuccess){
         if(waitTime <= 0){
            popUpIndex++;
            waitTime = 2f;
            isSuccess = false;
         }else{
            waitTime -= Time.deltaTime;
         }    
      }
      
   }

   public void SuccessTutorial1(){
      isSuccess = true;
   }

   public void OnClickSkipBtn(){
      popUpIndex = popUps.Length-1;
   }
}
