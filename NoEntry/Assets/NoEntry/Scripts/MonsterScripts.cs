using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterScripts : MonoBehaviour
{
    float tick = 0.0f;
   

    public GameObject fireDir; //발사 위치
    public GameObject bullet; //총알

    public float intervalDir;
    public float offset;

    public bool isTuto;
    public bool isTutoPlay;
    public bool isTutoFire;

    public GameObject tutoObj;

    float fireTime; //발사하는 동안 걸리는 시간.
    float rotationSpeed;
    float distance;

    public LineRenderer lineOfSight;
    public BoxCollider2D lineCollider;


    // Start is called before the first frame update
    void Start()
    {
        int difficulty = PlayerPrefs.GetInt("difficulty");
        GameBalance balance =  DataHelper.LoadData<GameBalance>("gameBalance", difficulty);
        fireTime = balance.monsterFireTime;
        rotationSpeed = balance.monsterRotationSpeed;
        distance = balance.monsterDistance;
        lineCollider.offset = new Vector2(0,balance.monsterColliderOffsetY);
        lineCollider.size = new Vector2(1,balance.monsterColliderOffSizeY);

        tick = fireTime;
        isTuto = true;
        isTutoPlay = false;
        isTutoFire = true;
        //Physics2D.queriesStartInColliders = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(!isTuto){
            //마우스 위치를 향하도록 발사위치 조정
            Vector2 difference = Camera.main.ScreenToWorldPoint(Input.mousePosition) - transform.position;
            float rotZ = Mathf.Atan2(difference.y, difference.x) * Mathf.Rad2Deg;
            fireDir.transform.rotation = Quaternion.Euler(0f,0f,rotZ+offset);
            fireDir.transform.position  = Vector2.zero+(difference.normalized * 2.0f);

            if(tick <= 0)
            {
                //총알 발사한다.
                Instantiate(bullet, fireDir.transform.position, fireDir.transform.rotation);
            
             tick = fireTime;
            }else
            {
                tick -= Time.deltaTime;
            }

            //회전.
            transform.Rotate(Vector3.forward * rotationSpeed * Time.deltaTime);
            lineOfSight.SetPosition(1, transform.position + transform.up * distance);
            lineOfSight.SetPosition(0, transform.position);

        }else{
            if(isTutoPlay){
                Vector2 difference = tutoObj.transform.position - transform.position;
                float rotZ = Mathf.Atan2(difference.y, difference.x) * Mathf.Rad2Deg;
                fireDir.transform.rotation = Quaternion.Euler(0f,0f,rotZ+offset);
                fireDir.transform.position  = Vector2.zero+(difference.normalized * 2.0f);
                if(isTutoFire){
                    Instantiate(bullet, fireDir.transform.position, fireDir.transform.rotation);
                    isTutoFire = false;
                }
            }
        }
    }
}
