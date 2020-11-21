import bpy


scene = bpy.data.scenes[0]
camera = [o for o in bpy.data.objects if o.type == "CAMERA"][0]
camera.location = (-4.427445888519287, -92.92864990234375, 60.707733154296875)
camera.rotation_euler = (1.275869607925415, 0.003415079554542899, -0.05574615299701691)
camera.data.clip_end = 200
camera.data.lens = 40

lights = [o for o in bpy.data.objects if o.type == "LIGHT"]

lights[0].location = (-24.40068244934082, -34.53718566894531, 61.14481735229492)
lights[0].data.energy = 50000
lights[0].data.shadow_soft_size = 10

figurine = [o for o in bpy.data.objects if o.type == "MESH" and o.name != "Tonella Figurine Socket"][0]
if "Monkey" not in figurine.name:
    bpy.ops.object.shade_smooth({'active_object': figurine})

if len(lights) == 1:
    # create light datablock, set attributes
    light_data = bpy.data.lights.new(name="light_2.80", type='POINT')
    light_data.energy = 30

    # create new object with our light datablock
    light_object = bpy.data.objects.new(name="light_2.80", object_data=light_data)

    # link light object
    bpy.context.collection.objects.link(light_object)

    # make it active 
    bpy.context.view_layer.objects.active = light_object
    
    lights.append(light_object)


lights[1].location = (66.81112670898438, 2.9324607849121094, 44.015838623046875)
lights[1].data.energy = 10000
lights[1].data.shadow_soft_size = 10

bpy.context.scene.render.filepath = bpy.context.blend_data.filepath.rsplit(".", 1)[0] + ".png"
bpy.context.scene.render.resolution_x = 768
bpy.context.scene.render.resolution_y = 1024
bpy.context.scene.render.film_transparent = True
bpy.ops.render.render(write_still=True)

bpy.ops.export_mesh.stl(filepath=bpy.context.blend_data.filepath.rsplit(".", 1)[0] + ".stl")

print("Done")

