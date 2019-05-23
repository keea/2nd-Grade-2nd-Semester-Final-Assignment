using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoundManager : MonoBehaviour
{
    public AudioSource BoomSound;
    // Start is called before the first frame update
    void Start()
    {
        BoomSound = gameObject.GetComponent<AudioSource>();
    }

    public void PlayBoom(){
        BoomSound.time = 0;
        BoomSound.Play();
    }
}
