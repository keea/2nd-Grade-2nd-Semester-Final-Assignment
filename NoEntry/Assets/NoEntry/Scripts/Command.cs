using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum STATE
{
    DEATH_TO_MONSTER, //몬스터에게 죽는 경우
    DEATH_TO_CRASH, //서로 부딪쳐서 죽는 경우
    DEATH_TO_BULLET, //공격에 맞아서 죽는 경우
}

[System.Serializable]
public class Command
{
    public STATE type;
}
