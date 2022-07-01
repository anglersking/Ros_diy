
(cl:in-package :asdf)

(defsystem "topic_test-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Message" :depends-on ("_package_Message"))
    (:file "_package_Message" :depends-on ("_package"))
    (:file "Num" :depends-on ("_package_Num"))
    (:file "_package_Num" :depends-on ("_package"))
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
    (:file "StringMessage" :depends-on ("_package_StringMessage"))
    (:file "_package_StringMessage" :depends-on ("_package"))
>>>>>>> cpp_py_topic_update
>>>>>>> “add_py_sub_pub”
  ))