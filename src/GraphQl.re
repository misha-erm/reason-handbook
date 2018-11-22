module GetAllPersons = [%graphql
  {|
    query allUsers($count: Int) {
      allUsers(count: $count) {
        firstName
        lastName
      }
    }
|}
];

module GetAllPersonsQuery = ReasonApollo.CreateQuery(GetAllPersons);

type action =
  | SetCount(int);

type state = {count: int};

let reducer = (action, _state) =>
  switch (action) {
  | SetCount(count) => ReasonReact.Update({count: count})
  };

let initialState = () => {count: 5};

let onInputChange = ({ReasonReact.send}, event) => {
    switch (ReactEvent.Form.target(event)##value) {
    | Some("") => SetCount(0) -> send
    | Some(value) => int_of_string(value) -> SetCount -> send
    | None => ()
    };
}

let component = ReasonReact.reducerComponent("GraphQl");

let make = _children => {
  ...component,
  initialState,
  reducer,
  render: self => {
    let usersQuery = GetAllPersons.make(~count=self.state.count, ());
    <div>
      {ReasonReact.string("Enter count")}
      <input
        autoFocus=true
        placeholder="enter count of persons"
        type_="text"
        value=(self.state.count->string_of_int)
        onChange=onInputChange(self)
      />
      <GetAllPersonsQuery variables=usersQuery##variables>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {ReasonReact.string("Loading")} </div>
               | Error(error) =>
                 <div> {ReasonReact.string(error##message)} </div>
               | Data(response) =>
                 <div>
                   {
                     switch (response##allUsers) {
                     | None => ReasonReact.string("No users fetched")
                     | Some(allUsers) =>
                       allUsers
                       ->Belt.Array.mapWithIndex((index, user) =>
                           <div key={index->string_of_int}>
                             {
                               switch (user) {
                               | None => ReasonReact.string("No user")
                               | Some(user) =>
                                 <>
                                   {ReasonReact.string(user##firstName)}
                                   {ReasonReact.string(user##lastName)}
                                 </>
                               }
                             }
                           </div>
                         )
                       ->ReasonReact.array
                     }
                   }
                 </div>
               }
           }
      </GetAllPersonsQuery>
    </div>;
  },
};