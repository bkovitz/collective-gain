(ns cgain.core
  (:refer-clojure :exclude [rand rand-int])
  (:require [clojure.tools.namespace.repl :refer [refresh]]
            #_[clojure.tools.trace :refer :all]
            [clojure.pprint :refer [pprint print-table]]
            #_[clojure.core.matrix :refer :all]
            [cgain.util :as util :refer [dd dde rand rand-int ifprob]]
            [incanter.stats :as stats])
  (:gen-class))

#_(set-current-implementation :vectorz)

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


(def x-size 20)
(def y-size 20)
(def world-dims [y-size x-size])
(def num-initial-organisms 5)
(def offspring-per-unit-sunlight (/ 1.0 8.0))
(def collective-gain 1.0)
(def collective-gain-per-square (/ collective-gain 8))

;; Generic matrix code

(defn make-matrix [[nrows ncols] fill]
  (let [row (vec (repeat ncols fill))]
    (vec (repeat nrows row))))

(defn mat-dims [mat]
  [(count mat) (count (get mat 0))])

(defn mget [mat coords]
  (get-in mat coords))

(defn mset [mat coords x]
  (assoc-in mat coords x))

(defn mset*f
  "f is a zero-argument function."
  [mat coordss f]
  (reduce #(mset %1 %2 (f)) mat coordss))

(defn seq-coords [mat]
  (let [[nrows ncols] (mat-dims mat)]
    (for [row (range nrows)
          col (range ncols)]
      [row col])))

(defn shuffle-coords [mat]
  (util/lazy-shuffle (seq-coords mat)))

(defn coordss-such-that
 ([pred? mat]
  (coordss-such-that pred? mat (seq-coords mat)))
 ([pred? mat coordss]
  (for [coords coordss
        :when (pred? (mget mat coords))]
    coords)))

(defn mupdate [mat coords f]
  (mset mat coords (f (mget mat coords))))

(defn mmap [f mat coordss]
  (reduce #(mset %1 %2 (f (mget %1 %2))) mat coordss))

(defn +torus [dim c1 c2]
  (mod (+ c1 c2) dim))

(defn +torus-coords [dims [row1 col1] [row2 col2]]
  [(+torus (get dims 0) row1 row2) (+torus (get dims 1) col1 col2)])

(defn coords-within [dims dist coords]
  (for [row-delta (range (- dist) (inc dist))
        col-delta (range (- dist) (inc dist))]
    (+torus-coords dims coords [row-delta col-delta])))

;; Making and altering the world

(defn random-initial-organism []
  (rand 0.05))

(defn make-organisms []
  (let [mat (make-matrix world-dims nil)]
    (mset*f mat (take num-initial-organisms (shuffle-coords mat))
                random-initial-organism)))
  
(defn make-world []
  {:organisms (make-organisms)
   :generation-num 0})

(defn get-organisms [world]
  (:organisms world))

(defn get-givers-and-takers [world]
  (:givers-and-takers world))

(defn get-claims [world]
  (:claims world))

(defn get-sunlight [world]
  (:sunlight world))

(defn get-claims [world]
  (:claims world))

(defn get-absorbed [world]
  (:absorbed world))

(defn get-offspring [world]
  (:offspring world))

(defn inc-generation-num [world]
  (update world :generation-num inc))

(defn get-g-at [world coords]
  (mget (get-organisms world) coords))

(defn get-organism-coordss [world]
  (->> (get-organisms world)
       (coordss-such-that some?)))

(defn get-num-organisms [world]
  (count (get-organism-coordss world)))

(defn develop-one-organism [g]
  (ifprob g :giver :taker))

(defn develop [world]
  (assoc world :givers-and-takers
    (let [organisms (get-organisms world)]
      (mmap develop-one-organism organisms (get-organism-coordss world)))))

(defn get-giver-coords [world]
  (->> (get-givers-and-takers world)
       (coordss-such-that #(= :giver %))))

(defn get-taker-coords [world]
  (->> (get-givers-and-takers world)
       (coordss-such-that #(= :taker %))))

(defn get-giver-and-taker-coords [world]
  (->> (get-givers-and-takers world)
       (coordss-such-that some?)))

(defn give-at [sunlight-mat coords]
  (reduce #(mupdate %1 %2 (partial + collective-gain-per-square))
          sunlight-mat
          (coords-within world-dims 2 coords)))

(defn make-sunlight [world]
  (assoc world :sunlight
    (reduce #(give-at %1 %2)
            (make-matrix world-dims 1.0)
            (get-giver-coords world))))

(defn get-sunlight-at [world coords]
  (mget (get-sunlight world) coords))

(defn claim-at [claims-mat coords]
  (reduce #(mupdate %1 %2 (fn [claims] (conj claims coords)))
          claims-mat
          (coords-within world-dims 1 coords)))

(defn claim-sunlight [world]
  (assoc world :claims
    (reduce #(claim-at %1 %2)
            (make-matrix world-dims [])
            (get-giver-and-taker-coords world))))

(defn get-claims-at [world coords]
  (mget (get-claims world) coords))

(defn absorb-at [world absorbed-map coords]
  (let [claims (get-claims-at world coords)
        num-claimants (count claims)
        sunlight (get-sunlight-at world coords)
        amt-absorbed (if (zero? num-claimants)
                         0.0
                         (/ sunlight num-claimants))]
    (reduce #(mupdate %1 %2 (partial + amt-absorbed))
            absorbed-map
            claims)))

(defn absorb-sunlight [world]
  (assoc world :absorbed
    (reduce #(absorb-at world %1 %2)
            (make-matrix world-dims 0.0)
            (seq-coords (get-claims world)))))

(defn amt-absorbed-at [world coords]
  (mget (get-absorbed world) coords))

(defn choose-num-offspring [world coords]
  (let [lambda (* (amt-absorbed-at world coords)
                  offspring-per-unit-sunlight)]
    (stats/sample-poisson 1 :lambda lambda)))

(defn offspring-location [offspring-mat parent-coords]
  (first
    (util/lazy-shuffle
      (coordss-such-that nil?
                         offspring-mat
                         (coords-within world-dims 2 parent-coords)))))

(defn make-one-offspring-for [world offspring-mat coords]
  (if-let [offspring-coord (offspring-location offspring-mat coords)]
    (assoc-in offspring-mat
              offspring-coord
              (get-g-at world coords))
    offspring-mat))

(defn make-offspring-for [world offspring-mat coords]
  (let [num-offspring (choose-num-offspring world coords)]
    (dd num-offspring)
    (reduce (fn [offspring-mat _]
              (make-one-offspring-for world offspring-mat coords))
            offspring-mat
            (range num-offspring))))

(defn get-offspring-coords [world]
  (coordss-such-that some? (get-offspring world)))

(defn make-offspring [world]
  (assoc world :offspring
    (reduce #(make-offspring-for world %1 %2)
            (make-matrix world-dims nil)
            (get-taker-coords world))))

(defn mutate-one-offspring [offspring-mat coords]
  (update-in offspring-mat
             coords
             #(util/clamp 0.0 1.0
               (+ % (stats/sample-normal 1 :mean 0.0 :sd 0.01)))))

(defn mutate-offspring [world]
  (update world :offspring
    (fn [orig-offspring]
      (reduce #(mutate-one-offspring %1 %2)
              orig-offspring
              (get-offspring-coords world)))))

(defn offspring-replace-parents [world]
  (if-let [offspring (get-offspring world)]
    (-> world
        (assoc :organisms offspring)
        (dissoc :offspring))
    world))

;; Printing

(defn mstr [mat]
  (clojure.string/join \newline
    (for [row mat]
      (str "  " (clojure.string/join \space row)))))

(defn print-world [world]
  (println "generation-num" (:generation-num world))
  (println "organisms\n" (mstr (get-organisms world)))
  (println "development\n" (mstr (get-givers-and-takers world)))
  (println "sunlight\n" (mstr (get-sunlight world)))
  (println "absorption\n" (mstr (get-absorbed world)))
  (println "offspring\n" (mstr (get-offspring world)))
  (println "num-organisms" (get-num-organisms world))
  (println)
  world)

;; Evolution

(defn run-one-generation [world]
  (let [result (->> world
                    offspring-replace-parents
                    develop
                    make-sunlight
                    claim-sunlight
                    absorb-sunlight
                    make-offspring
                    mutate-offspring
                    inc-generation-num)]
    result
    #_(print-world result)))

(defn run []
  (dorun 100
    (->> (iterate run-one-generation (make-world))
         (take-while #(> (get-num-organisms %) 0))
         (map print-world))))
