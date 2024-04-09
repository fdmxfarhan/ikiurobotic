import cv2 as cv
import numpy as np
import os



pre_path = r'E:\frame'
b = 0

for a in range(197):

    b += 1
    raw_path = pre_path + str(b)
    main_img_path = raw_path + '.png'
    main_txt_path = raw_path + '.txt'

    #check point 01:
    print('this is raw_path:', raw_path)
    print('this is main_img_path:', main_img_path)
    print('this is main_txt_path:', main_txt_path)


    main_img = cv.imread(main_img_path, cv.IMREAD_UNCHANGED)
    obj_img = cv.imread(r'C:\Users\asus\Desktop\r_p_g.jpg', cv.IMREAD_UNCHANGED)


   
    print(main_img.shape)
    print(obj_img.shape)

    
    result = cv.matchTemplate(main_img, obj_img, cv.TM_CCOEFF_NORMED)


 

    min_val, max_val, min_loc, max_loc = cv.minMaxLoc(result)

    print('Best match top left position: %s' % str(max_loc))
    print('Best match confidence: %s' % max_val)


    threshold = 0.4
    if max_val >= threshold:
        print('Found Object.')

    
        obj_w = obj_img.shape[1]
        obj_h = obj_img.shape[0]
    

   
        top_left = max_loc
        bottom_right = (top_left[0] + obj_w, top_left[1] + obj_h)
        top_right = (top_left[0] + obj_w, top_left[1])
        bottom_left = (top_left[0], top_left[1] + obj_h)
        x_center = (top_left[0] + bottom_right[0]) / 2.0
        y_center = (top_left[1] + bottom_right[1]) / 2.0
        x_center /= main_img.shape[1]
        y_center /= main_img.shape[0]
        width = obj_w / main_img.shape[1]
        height = obj_h / main_img.shape[0]
    
        cv.rectangle(main_img, top_left, bottom_right, 
                        color = (0, 255, 0), thickness = 2, lineType = cv.LINE_4)
    
    
        final_text = '0 ' + str(x_center) + ' ' + str(y_center) + ' ' + str(width) + ' ' + str(height)
    
        with open(main_txt_path, 'w') as file:
    
            file.write(final_text)


    else:
        print('Object not found.')



