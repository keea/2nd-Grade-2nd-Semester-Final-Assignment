using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    const int MAX_CREATE_POSTION = 4;
    public GameObject[] gameObj;
    public float createTime;
    float tick = 0.0f;
    Vector2[] pos;
    public GameObject respownPos;

    public int createMaxNum;
    int countCreateObj;
    Vector2 createPosition;

    ResonForDeath result;

    public List<Command> cmdList = new List<Command>();

    public GameObject backgroundLiner;

    // Start is called before the first frame update
    void Start()
    {
        pos = new Vector2[MAX_CREATE_POSTION];
        pos[0] = new Vector2(-12f, 6f);
        pos[1] = new Vector2(-12f, -6f);
        pos[2] = new Vector2(12f, -6f);
        pos[3] = new Vector2(12f, 6f);

        result = new ResonForDeath();

        CreateRandomRespown();
        respownPos.SetActive(true);
        UnVisibleBackgroundLiner();
    }

    // Update is called once per frame
    void Update()
    {
        if(cmdList.Count > 0)
        {
            switch (cmdList[0].type)
            {
                case STATE.DEATH_TO_MONSTER:
                result.deathToMonster+=1;
                break;

                case STATE.DEATH_TO_CRASH:
                result.deathToCrash+=1;
                break;

                case STATE.DEATH_TO_BULLET:
                result.deathToAttack+=1;
                break;
            }
            cmdList.RemoveAt(0);
            countCreateObj-=1;
            respownPos.SetActive(true);
        }
        CreateMoveObject();
    }

    //움직이는 객체 생성.
    void CreateMoveObject()
    {
        if(countCreateObj < createMaxNum){
            if(tick <= 0.0){
                //랜덤한 위치에서 생성.
                //Instantitate(생성할오브젝트, postion, rotation)
                //Quaternion.identity = rotation(회전각)이 0,0,0 임을 의미.
                //int randNum = Random.Range(0, (MAX_CREATE_POSTION - countCreateObj)-1);
                Instantiate(gameObj[Random.Range(0, gameObj.Length)], createPosition, Quaternion.identity);
                tick = createTime;
                countCreateObj++;

                //다음에 생성할 위치.
                CreateRandomRespown();

            }else{
                //respownPos.SetActive(true);
                tick -= Time.deltaTime;
            }
        }else{
            tick = createMaxNum;
        }
    }

    void CreateRandomRespown(){
        int randNum = Random.Range(0, (MAX_CREATE_POSTION));
        createPosition = pos[randNum];
        respownPos.transform.position = createPosition;

        if(countCreateObj >= createMaxNum)
            respownPos.SetActive(false);
    }

    public void AddActions(Command cmd){
        cmdList.Add(cmd);
    }

    public void VisibleBackgroundLiner(Color color, float width){
        LineRenderer lineRenderer = backgroundLiner.GetComponent<LineRenderer>();
        lineRenderer.startColor = color;
        lineRenderer.endColor = color;
        lineRenderer.enabled = true;
        lineRenderer.startWidth = width;
        lineRenderer.endWidth = width;
    }

    public void UnVisibleBackgroundLiner(){
        LineRenderer lineRenderer = backgroundLiner.GetComponent<LineRenderer>();
        lineRenderer.enabled = false;

        lineRenderer.startWidth = 0.25f;
        lineRenderer.endWidth = 0.25f;
    }
}
