using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TransitionsStart : MonoBehaviour
{
    public Material mat;
    void Start()
    {
        StartCoroutine("IntoTheScene");
    }  

    IEnumerator IntoTheScene(){
        Material TransitionsMaterial = mat;
        float cutOff = 1.0f;
        while(cutOff > 0.0f){
            cutOff -= 0.1f;
            TransitionsMaterial.SetFloat("_Cutoff", cutOff);
            yield return new WaitForSeconds(0.1f);
        }
    }
}
