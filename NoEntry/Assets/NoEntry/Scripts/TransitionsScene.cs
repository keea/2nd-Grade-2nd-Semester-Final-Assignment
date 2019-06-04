using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TransitionsScene : MonoBehaviour
{
    public Material TransitionsMaterial;

    void Start(){
        TransitionsMaterial.SetFloat("_Cutoff", 0.0f);
    }

    void OnRenderImage(RenderTexture src, RenderTexture dst){
        Graphics.Blit(src, dst, TransitionsMaterial);
    }
}
