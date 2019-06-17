using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TitleManager : MonoBehaviour
{
    public Material mat;

    void Awake(){
        PlayerPrefs.SetInt("isClearTuto", 0);
    }

    public void GoGameScene(int difficulty){
        Debug.Log("난이도 : " + difficulty);
        Time.timeScale = 1;
        PlayerPrefs.SetInt("difficulty", difficulty);
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
