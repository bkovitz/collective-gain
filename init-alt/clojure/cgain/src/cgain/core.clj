(ns cgain.core
  (:gen-class))

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (println "Hello, World!"))

;; World:
;;   Squares (a torus)
;;     Each has an x-y coordinate
;;     Each gets sunlight
;;     Each gets protection
;;   Organisms:
;;     Each has a square (up to one per square)
;;     Each has a g
;;     Each gets a type: :giver or :taker
;;     Each absorbs sunlight
;;     Givers give protection
;;     Takers put children into nearby squares, limited by sunlight absorbed


(def x-size 30)
(def y-size 30)

(defn make-squares []
  )


(defn make-world []
  {:squares (make-squares)
   :organisms (make-organisms)})
