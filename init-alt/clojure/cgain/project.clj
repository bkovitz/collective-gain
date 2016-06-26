(defproject cgain "0.1.0-SNAPSHOT"
  :description "Collective Gain: an evolutionary simulation"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.8.0"]
                 [net.mikera/core.matrix "0.52.1"]
                 [net.mikera/vectorz-clj "0.44.1"]]
  :main ^:skip-aot cgain.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
