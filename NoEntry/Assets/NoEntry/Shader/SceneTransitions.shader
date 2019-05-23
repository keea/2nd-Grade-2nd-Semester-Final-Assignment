Shader "Custom/SceneTransitions"
{
    Properties
    {
        [PerRendererData]_MainTex ("Sprite Texture", 2D) = "white" {}
        _Color ("Tint", Color) = (1,1,1,1)
        _Cutoff("cutoff", Range(0.0, 1.0)) = 0.0
    }

    SubShader
    {
        Tags
        {
            "Queue"="Transparent"
            "IgnoreProjector"="True"
            "RenderType"="Transparent"
            "PreviewType"="Plane"
            "CanUseSpriteAtlas"="True"
        }

        Cull Off
        Lighting Off
        ZWrite Off
        Blend One OneMinusSrcAlpha

        Pass
        {
        CGPROGRAM
            #pragma vertex SimpleVert
            #pragma fragment SimpleFrag
            #pragma target 2.0
            #pragma multi_compile_instancing
            #pragma multi_compile _ PIXELSNAP_ON
            #pragma multi_compile _ ETC1_EXTERNAL_ALPHA
            #include "UnitySprites.cginc"

            v2f SimpleVert(appdata_t v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.texcoord = v.texcoord;
                return o;
            }

            float _Cutoff;

            fixed4 SimpleFrag(v2f i) : SV_Target
            {
                // if(i.texcoord.x < _Cutoff)
                //     return _Color;

                if(0.5-abs(i.texcoord.y - 0.5) < abs(_Cutoff) * 0.5)
                    return _Color;

                return SampleSpriteTexture (i.texcoord);
            }
        ENDCG
        }
    }
}