# ObjectPoolingProject

Developed with Unreal Engine 5.1.1.

<br>

Spawning and destorying actors/objects can be an expensive process in Unreal Engine, and it can cause major hitches and lag spikes if there are multiple actors spawning/getting destoryed at the same time. This plugin allows actors/objects to be spawned from an object pool, reducing the overall performance impact. This is effective for games that require constant spawning/destroying of actors like Bullet Hell games and RTS games.

<br>

<h1>
Performance demonstration
</h1>



In this test project, 1250 projectiles are present, and whenever they are destroyed they are respawned again.

**Please note that this is an extreme example for when this plugin can be useful. The performance gains might not be as high as described below for other cases.**

In the first video, the plugin is disabled and projectiles are being spawned and destroyed the conventional way, while the second video the object pooling plugin is used. The graph below shows the **frame time**, so the higher it is the worse the performance. There are 2 main benefits to using the plugin for this example. The first benefit is that the FPS is generally higher using the pooling plugin, by about 25%. The second benefit, which is more important in this case, is that the hitching and lag spikes are significantly reduced and almost eliminated completely.



<br>
<br>

https://github.com/user-attachments/assets/3db6c0c9-4ab8-4f4d-81d9-a097f0a7fd16


https://github.com/user-attachments/assets/a8be03de-dbe7-4971-b8aa-4910426eeeef


<h1> How to use</h1>

<h2>Object Pooling Subsystem</h2>

![image](https://github.com/user-attachments/assets/9cda1f4a-d80f-4580-bd41-4255dd638977)

The Object Pooling Subsystem can be accessed globally in any BP or C++ class. The lifetime is handled automatically so you don't have to think about it. 

The initialize pool function is used to prespawn actors. These actors are stored and ready to use later whenever needed. You usually want to do this at the start of the level behind a loading screen. To use the stored actors, just call 'Spawn From Pool' or 'Spawn Multiple From Pool' and the actors are spawned in. If there are not enough actors ready in the pool, then normal spawning is used instead and the new actor(s) are added to the pool for later use.

