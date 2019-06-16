using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TutorialBall : MonoBehaviour
{
    Vector3 target;
    public float speed;
    Vector3 firstPostion; //첫번째 위치를 저장하는 함수

    public int TutoType;

    public GameObject effect;

    public GameObject tutoText;

    private SoundManager soundManager;

    void Start()
    {
        firstPostion = transform.position;
        target = Vector3.zero;
        soundManager = GameObject.Find("SoundManager").GetComponent<SoundManager>();

        if(TutoType == 3){
            MonsterScripts monster = GameObject.Find("Monster").GetComponent<MonsterScripts>();
            monster.isTutoPlay = true;
        }
    }

    // Update is called once per frame
    void Update()
    {
        float step = speed * Time.deltaTime;
        transform.position = Vector3.MoveTowards(transform.position, target, step);

        if(transform.position == target){
            transform.GetComponent<TrailRenderer>().enabled = false;
            if(TutoType == 0)
                transform.position = firstPostion;
        }


    }

    private void OnMouseDrag()
    {
        if(TutoType == 0){
            transform.GetComponent<TrailRenderer>().enabled = true;

            Vector2 mousePosition = new Vector2(Input.mousePosition.x,
                Input.mousePosition.y);

            Vector2 objPostion = Camera.main.ScreenToWorldPoint(mousePosition);
            transform.position = objPostion;
        }
        
    }

    private void OnMouseUp()
    {
        if(TutoType == 0){
            Debug.Log("마우스 업");
            tutoText.GetComponent<TextMesh>().color = new Color(0, 1, 1, 1);
            GameObject.Find("TutorialManager").GetComponent<TutorialManager>().SuccessTutorial1();
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.tag.Equals("Enemy"))
        {
            if(TutoType == 1){
                soundManager.PlayBoom();
                speed = 0;

                Instantiate(effect, transform.position, Quaternion.identity);
                GetComponent<SpriteRenderer>().enabled = false;
                transform.GetComponent<TrailRenderer>().enabled = false;
                if(tutoText!=null)
                    tutoText.GetComponent<TextMesh>().color = new Color(0, 1, 1, 1);
                GameObject.Find("TutorialManager").GetComponent<TutorialManager>().SuccessTutorial1();
            }
        }else if(collision.gameObject.tag.Equals("MoveObject")){
            if(TutoType == 2){
                 soundManager.PlayBoom();
                 speed = 0;

                Instantiate(effect, transform.position, Quaternion.identity);
                GetComponent<SpriteRenderer>().enabled = false;
                transform.GetComponent<TrailRenderer>().enabled = false;
                if(tutoText!=null)
                    tutoText.GetComponent<TextMesh>().color = new Color(0, 1, 1, 1);
                GameObject.Find("TutorialManager").GetComponent<TutorialManager>().SuccessTutorial1();
            }
        }else if(collision.gameObject.tag.Equals("Bullet")){
            if(TutoType == 3){
                 soundManager.PlayBoom();
                 speed = 0;

                Instantiate(effect, transform.position, Quaternion.identity);
                GetComponent<SpriteRenderer>().enabled = false;
                transform.GetComponent<TrailRenderer>().enabled = false;
                if(tutoText!=null)
                    tutoText.GetComponent<TextMesh>().color = new Color(0, 1, 1, 1);
                
                GameObject.Find("TutorialManager").GetComponent<TutorialManager>().SuccessTutorial1();
            }
        }
    }
}
