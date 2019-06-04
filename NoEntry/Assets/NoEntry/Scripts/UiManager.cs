using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UiManager : MonoBehaviour
{
    public Text hpText;
    public Text timeText;

    public GameObject GamePanel;
    public GameObject ResultPanel;

    public Text resultTime;
    public Text dtMonster;
    public Text dtBullet;
    public Text dtCrash;

    Animator hpTextAnim;

    void Start(){
        hpTextAnim = hpText.gameObject.GetComponent<Animator>();
    }

    public void ShowGamePanel(){
        ResultPanel.SetActive(false);
        GamePanel.SetActive(true);
    }

    public void ShowResultPanel(){
        ResultPanel.SetActive(true);
        GamePanel.SetActive(false);
    }

    public void setResultPanel(float time, ResonForDeath death){
        string timeStr;
        timeStr = ""+time.ToString("00.00");
        timeStr = timeStr.Replace(".",":");

        resultTime.text = timeStr;
        dtBullet.text = "Death To Bullet : "+death.deathToAttack.ToString();
        dtMonster.text = "Death To Monster : "+death.deathToMonster.ToString();
        dtCrash.text = "Death To Crash : "+death.deathToCrash.ToString();
    }

    public void setTimeText(float time){
        string timeStr;
        timeStr = ""+time.ToString("00.00");
        timeStr = timeStr.Replace(".",":");
        timeText.text = timeStr;
    }
    public void setHpText(int hp){
        hpTextAnim.SetTrigger("bounce");
        if(hp<0)
            hp = 0;
        hpText.text = hp.ToString();
    }
}
