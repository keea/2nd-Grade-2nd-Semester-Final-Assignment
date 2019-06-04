using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TitleManager : MonoBehaviour
{
    public Material mat;

    void Awake(){
        Screen.SetResolution(Screen.width, Screen.width * 16/9, true);
    }

    public void GoGameScene(){
        Time.timeScale = 1;
        StartCoroutine("Transitions");
    }

    IEnumerator Transitions(){
        Material TransitionsMaterial = mat;
        float cutOff = 0.0f;
        while(cutOff < 1.0f){
            cutOff += 0.1f;
            TransitionsMaterial.SetFloat("_Cutoff", cutOff);
            yield return new WaitForSeconds(0.1f);
        }
        SceneManager.LoadScene("GameScene");
    }
}
