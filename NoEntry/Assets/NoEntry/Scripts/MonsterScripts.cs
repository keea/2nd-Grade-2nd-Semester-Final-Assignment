using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterScripts : MonoBehaviour
{
    float tick = 0.0f;
    public float fireTime; //발사하는 동안 걸리는 시간.

    public GameObject fireDir; //발사 위치
    public GameObject bullet; //총알

    public float intervalDir;
    public float offset;


    // Start is called before the first frame update
    void Start()
    {
        tick = fireTime;
    }

    // Update is called once per frame
    void Update()
    {
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
    }
}
