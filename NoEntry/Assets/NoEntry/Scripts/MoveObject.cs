using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public enum OBJ_COLCOR{
    BLUE,
    GREEN,
    PINK,
}

public class MoveObject : MonoBehaviour
{
    // Start is called before the first frame update
    Vector2 target;
    public float speed;
    float OnMouseDonwTime = 0.0f;

    public GameObject effect;

    private GameManager GM;

    Command cmd;

    public OBJ_COLCOR objColor;
    public GameObject[] spltters;
    Color obj_color;

    Animator anim;

    private Shake shake;

    void Start()
    {
        target = Vector2.zero;
        shake = GameObject.FindGameObjectWithTag("ScreenShake").GetComponent<Shake>();
        GM = GameObject.Find("GameManager").GetComponent<GameManager>();
        anim = transform.GetComponent<Animator>();
        cmd = new Command();

        switch(objColor){
            case OBJ_COLCOR.BLUE:
            obj_color = new Color(0, 1, 1, 1);
            break;

            case OBJ_COLCOR.PINK:
            obj_color = new Color(1, 0, 1, 1);
            break;

            case OBJ_COLCOR.GREEN:
            obj_color = new Color(0, 1, 0, 1);
            break;
        }
    }

    // Update is called once per frame
    void Update()
    {
        float step = speed * Time.deltaTime;
        transform.position = Vector3.MoveTowards(transform.position, target, step);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag.Equals("Enemy"))
        {
            Destroy(gameObject); //자기 자신 제거
            cmd.type = STATE.DEATH_TO_MONSTER;   
        }
        else if(collision.gameObject.tag.Equals("Bullet")){
            Destroy(gameObject); //자기 자신 제거
            cmd.type = STATE.DEATH_TO_BULLET;
        }
        else if(collision.gameObject.tag.Equals("MoveObject")){
            Destroy(gameObject); //자기 자신 제거
            cmd.type = STATE.DEATH_TO_CRASH;
        }

        GM.AddActions(cmd);
        Debug.Log("color : " + obj_color);
        shake.CamShake();
        GameObject temp = Instantiate(spltters[Random.Range(0, spltters.Length)], transform.position, Quaternion.identity);
        temp.GetComponent<SpriteRenderer>().color = obj_color;
        Instantiate(effect, transform.position, Quaternion.identity);
    }

    private void OnMouseDrag()
    {
        GM.VisibleBackgroundLiner(obj_color, OnMouseDonwTime+0.25f);
        //마우스 드래그
        Debug.Log(OnMouseDonwTime);
        anim.SetBool("isPingPongZoom", true);
        OnMouseDonwTime += Time.deltaTime;

        Vector2 mousePosition = new Vector2(Input.mousePosition.x,
            Input.mousePosition.y);

        Vector2 objPostion = Camera.main.ScreenToWorldPoint(mousePosition);
        transform.position = objPostion;
    }

    //마우스 뗄 떼 초기화.
    private void OnMouseUp()
    {
        GM.UnVisibleBackgroundLiner();
        anim.SetBool("isPingPongZoom", false);
        OnMouseDonwTime = 0.0f;
    }
}
