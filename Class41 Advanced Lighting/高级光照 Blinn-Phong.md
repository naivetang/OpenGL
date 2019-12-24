- 冯氏光照组成(https://learnopengl-cn.github.io/02%20Lighting/02%20Basic%20Lighting/)：
    - 环境光
    - 漫反射
    - 镜面光照
- 冯氏光照弊端：镜面光照强度由视线方向与光照反射光线夹角决定，夹角越小，镜面光越强。夹角大于90度，点积结果为负数，镜面光强度为0
- Blinn-Phong可以解决镜面光强度为0问题，它计算镜面光强度大小由半程向量与法线夹角决定，夹角越小，镜面光越强。
    - 半程向量：光线与视线夹角的一半方向，上的一个单位向量。半程向量与法线夹角始终小于90°。
    - 半程向量计算：光线的方向向量和观察向量加到一起，并将结果正规化(Normalize)。
        vec3 lightDir   = normalize(lightPos - FragPos);
        vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);