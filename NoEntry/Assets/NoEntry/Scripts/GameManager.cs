using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    const int MAX_CREATE_POSTION = 4;
    public GameObject[] gameObj;
    public float createTime;
    float tick = 0.0f;
    public GameObject[] respownPos;
    public Sprite respownSpriteOn;
    public Sprite respownSpriteOff;

    public int createMaxNum;
    int countCreateObj;
    int createPositionIndex;

    ResonForDeath result;

    public List<Command> cmdList = new List<Command>();

    public GameObject backgroundLiner;

    Dictionary<int, GameObject> moveObjects;

    public int maxHp;
    int hp;

    float time;

    private UiManager uiManager;

    bool isShowResult;

    // Start is called before the first frame update
    void Start()
    {
        Time.timeScale = 1;
        result = new ResonForDeath();
        moveObjects = new Dictionary<int, GameObject>();
        uiManager = GameObject.Find("UiManager").GetComponent<UiManager>();
        uiManager.ShowGamePanel();
        hp = maxHp;
        uiManager.setHpText(hp);
        time = 0;

        CreateRandomRespown();
        UnVisibleBackgroundLiner();
        isShowResult = false;
    }

    // Update is called once per frame
    void Update()
    {
        uiManager.setTimeText(time);

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
                    result.deathToCrash+=1;
                break;
            }

            hp-=1;
            uiManager.setHpText(hp);

            GameObject deleteObj = moveObjects[cmdList[0].instanceId];
            bool rmResult = moveObjects.Remove(cmdList[0].instanceId);
            Destroy(deleteObj);

            cmdList.RemoveAt(0);
            countCreateObj-=1;
            
        }

        if(hp <= 0)
        {
            StartCoroutine("SlowMotion");
            AllDeath();
            if(isShowResult)
                SetResultUiPanel();
        }else
        {
           
            CreateMoveObject();
            time+=Time.deltaTime;
        }

        // if(hp<=0){
        //     StartCoroutine("SlowMotion");
        //     if(isShowResult)
        //         SetResultUiPanel();
        // }
        // else
        // {
        //     if(cmdList.Count > 0)
        // {
        //     switch (cmdList[0].type)
        //     {
        //         case STATE.DEATH_TO_MONSTER:
        //         result.deathToMonster+=1;
        //         break;

        //         case STATE.DEATH_TO_CRASH:
        //         result.deathToCrash+=1;
        //         break;

        //         case STATE.DEATH_TO_BULLET:
        //         result.deathToAttack+=1;
        //         break;
        //     }
        //     hp-=1;
        //     moveObjects.Remove(cmdList[0].instanceId);
        //     cmdList.RemoveAt(0);
        //     countCreateObj-=1;
        //     uiManager.setHpText(hp);
        // }
        // CreateMoveObject();
        // time+=Time.deltaTime;
        // }
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
                
                GameObject obj = Instantiate(gameObj[Random.Range(0, gameObj.Length)], respownPos[createPositionIndex].transform.position, Quaternion.identity);
                tick = createTime;
                countCreateObj++;
                respownPos[createPositionIndex].GetComponent<SpriteRenderer>().sprite = respownSpriteOff;
                Debug.Log("인스턴스 아이디 : " + obj.GetInstanceID());
                moveObjects.Add(obj.GetInstanceID(), obj);
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

    void AllDeath()
    {
        //Key값으로 Loop(반복문)를 돌리고 해당 key만 삭제
        foreach(int sKey in moveObjects.Keys.ToList()){
            GameObject gameObj = moveObjects[sKey];
            gameObj.GetComponent<MoveObject>().DestroyObject();

            moveObjects.Remove(sKey);
            Destroy(gameObj);
        }
    }

    void SetResultUiPanel(){
        uiManager.setResultPanel(time, result);
        uiManager.ShowResultPanel();
    }

    void CreateRandomRespown(){
        int randNum = Random.Range(0, (MAX_CREATE_POSTION));
        createPositionIndex = randNum;
        respownPos[createPositionIndex].GetComponent<SpriteRenderer>().sprite = respownSpriteOn;
        //respownPos.transform.position = createPosition;
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

    IEnumerator SlowMotion(){
        Time.timeScale = 0.1f;
        yield return new WaitForSeconds(0.3f);
        Time.timeScale = 0;
        isShowResult = true;
    }

    public void Restart(){
        SceneManager.LoadScene("GameScene");
    }
}
