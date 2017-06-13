#### 3D Rolling Ball

#### Use C++ and openGL (glfw)

![](./screenShot/normal.png)
---
#### Dependencies :

- c++11 ( for lambda expression and shared_ptr )
- opengl + glfw


---

#### Build :

	./make
	./rollingBall


####Features :

- Blinn-Phong Lighting
- Rotating Skybox
- Shadow effect
- Signed Distance Fields text rendering
- Ping-Pong shader
- Water surface
- simple HDR and Bloom effect
- Sun lens flare
---
#### ScreenShots :


#####Shadow effect : use shadowMap to render shadow and PCF to avoid sharp shadow edge

Before PCF             |  After PCF
-------------------------|-------------------------
![](./screenShot/before_pcf.png)   |  ![](./screenShot/after_pcf.png)

---
#####Signed Distance Fields text rendering And Ping-Pong shader for background blurring

For text rendering, I use the so called SDF technology instead of freetype

support text mode : Normal, Outlined, Glow, Drop-shadow ( all with hover effect )

![](./screenShot/menu.png)

For background blurring, I use ping-pong shader, which takes two framebuffers.

without ping-pong shader, use 2D gauss kernel             |  using ping-pong shader with two 1D gauss kernels
-------------------------|-------------------------
![](./screenShot/beforePingpong.png)   |  ![](./screenShot/menu.png)

----
#### Water Effect : reflection + refraction + dudvMap + normalMap

reflection + refraction :

![](./screenShot/ref.png)

dudvMap :

![](./screenShot/distortion.png)

normalMap :

![](./screenShot/normalMap.png)

----

#### Simple HDR +  Bloom Effect :

![](./screenShot/bloom.png)

---
#### Sun lens flare :

![](./screenShot/lensFlare.png)