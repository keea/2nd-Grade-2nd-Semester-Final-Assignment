using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    public float speed;
    public float lifeTime;


    // Start is called before the first frame update
    void Start()
    {
        //Invoke("실행함수",지연시간);  lifeTime 후에 함수가 호출된다.
        Invoke("DestoryBullet", lifeTime);
    }

    void Update()
    {
        //y축 방향으로 이동.
        transform.Translate(Vector2.up * speed * Time.deltaTime);
    }

    void DestoryBullet(){
        Destroy(gameObject);
    }
}
