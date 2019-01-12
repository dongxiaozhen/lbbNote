package main

import (
	"fmt"
	"log"

	"github.com/boltdb/bolt"
)

func main() {
	db, err := bolt.Open("my.db", 0600, nil)
	if err != nil {
		log.Fatal(err)
		return
	}
	defer db.Close()

	err = db.Update(func(tx *bolt.Tx) error {
		b, err := tx.CreateBucketIfNotExists([]byte("myBucket2"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		b.FillPercent = float64(100)
		err = b.Put([]byte("2first"), []byte("2libinbin"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		err = b.Put([]byte("2second"), []byte("2lixinyuan"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		err = b.Put([]byte("2thirst"), []byte("2liqinyuan"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		for i := 0; i < 6; i++ {
			key := fmt.Sprintf("lbb2keyx%d", i)
			value := fmt.Sprintf("lbb2Valuex%d", i)
			err = b.Put([]byte(key), []byte(value))
			if err != nil {
				log.Fatal(err)
				return err
			}
		}

		v := b.Get([]byte("2thirst"))
		log.Printf("2thirst--%s", v)

		nb, err := b.CreateBucket([]byte("myBucket3"))
		if err != nil {
			return err
		}
		nb.FillPercent = float64(100)

		err = nb.Put([]byte("nthirst"), []byte("rnliqinyuan"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		for i := 0; i < 5; i++ {
			key := fmt.Sprintf("nnkeyx%d", i)
			value := fmt.Sprintf("nnValuex%d", i)
			err = nb.Put([]byte(key), []byte(value))
			if err != nil {
				log.Fatal(err)
				return err
			}
		}

		return nil
	})

	err = db.Update(func(tx *bolt.Tx) error {
		b, err := tx.CreateBucketIfNotExists([]byte("myBucket1"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		b.FillPercent = float64(100)
		err = b.Put([]byte("1first"), []byte("1libinbin"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		err = b.Put([]byte("1second"), []byte("1lixinyuan"))
		if err != nil {
			log.Fatal(err)
			return err
		}
		err = b.Put([]byte("1thirst"), []byte("1liqinyuan"))
		if err != nil {
			log.Fatal(err)
			return err
		}

		v := b.Get([]byte("thirst"))
		log.Printf("thirst--%s", v)
		return nil
	})
}
